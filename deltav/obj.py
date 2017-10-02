import warnings, os

from aegame import *
from mash3D import *

def load_texture(filename):
    return Texture().load(os.path.join(base_path(), 'data', filename))

class MaterialGroup(object):
    __slots__ = ['material', 'buffer']

    def __init__(self, material):
        self.material = material
        self.buffer = Buffer().create_empty()

    def __del__(self):
        self.buffer.close()

class Mesh(object):
    __slots__ = ['name', 'groups', 'polygons']

    def __init__(self, name):
        self.name = name
        self.groups = []
        self.polygons = []

    @profile("obj.py", "Mesh.draw")
    def draw(self):
        for group in self.groups:
            group.buffer.draw_ex(group.material.cast())

        return self

    @profile("obj.py", "Mesh.get_aabbox")
    def get_aabbox(self):
        if self.groups:
            boxes = [group.buffer.aabbox for group in self.groups]

            min_v = boxes[0][0].copy()
            max_v = boxes[0][1].copy()

            for box in boxes:
                min_v.imin_vec(box[0])
                max_v.imax_vec(box[1])

            return (min_v, max_v)
        else:
            return (Vec3(), Vec3())

class OBJ(object):
    __slots__ = ['materials', 'meshes', 'mesh_list']

    @profile("obj.py", "OBJ.__init__")
    def __init__(self, filename):
        self.materials = {}
        self.meshes = {}
        self.mesh_list = []

        mesh = group = material = None

        vertices = [[0., 0., 0.]]
        normals = [[0., 0., 0.]]
        tex_coords = [[0., 0.]]

        for line in open(os.path.join(base_path(), "data", filename), "r"):
            if line.startswith('#'): continue

            values = line.split()
            if not values: continue

            if values[0] == 'v':
                vertices.append(map(float, values[1:4]))
            elif values[0] == 'vn':
                normals.append(map(float, values[1:4]))
            elif values[0] == 'vt':
                tex_coords.append(map(float, values[1:3]))
            elif values[0] == 'mtllib':
                self.load_material_library(values[1])
            elif values[0] in ('usemtl', 'usemat'):
                material = self.materials.get(values[1], None)
                if material is None:
                    warnings.warn('Unknown material: %s' % values[1])
                if mesh is not None:
                    group = MaterialGroup(material)
                    mesh.groups.append(group)
            elif values[0] == 'o':
                mesh = Mesh(values[1])
                self.meshes[mesh.name] = mesh
                self.mesh_list.append(mesh)
                group = None
            elif values[0] == 'f':
                if mesh is None:
                    mesh = Mesh('')
                    self.mesh_list.append(mesh)
                if material is None:
                    material = Material()
                if group is None:
                    group = MaterialGroup(self, material)
                    mesh.groups.append(group)

                # For fan triangulation, remember first and latest vertices
                v1 = vlast = None
                points = []

                for i, v in enumerate(values[1:]):
                    v_index, t_index, n_index = \
                        (map(int, [j or 0 for j in v.split('/')]) + [0, 0])[:3]

                    if v_index < 0: v_index += len(vertices) - 1
                    if t_index < 0: t_index += len(tex_coords) - 1
                    if n_index < 0: n_index += len(normals) - 1

                    vertex = tex_coords[t_index] + normals[n_index] + vertices[v_index]

                    if i >= 3: # fan triangulation (first/latest vertices)
                        verts, inds = group.buffer.lock()
                        for val in v1 + vlast: verts.append_flt(val)
                        group.buffer.unlock(verts, inds)

                    group.buffer.append_vertex(
                        Vec2(*tex_coords[t_index]),
                        None,
                        Vec3(*normals[n_index]),
                        Vec3(*vertices[v_index]))

                    if i == 0: v1 = vertex
                    vlast = vertex

                    points.append(Vec3(*vertices[v_index]))
                mesh.polygons.append(PlaneWinding().from_polygon_points(points))

        for mesh in self.mesh_list:
            for group in mesh.groups:
                # TODO: this ratio is an arbitrary guess
                if group.buffer.unique_vertex_ratio() < 0.5:
                    group.buffer.weld()
                    group.buffer.vertex_cache_optimize()
                    group.buffer.shrink_indices()

                group.buffer.compute_aabbox = True

    @profile("obj.py", "OBJ.load_material_library")
    def load_material_library(self, filename):
        material = None

        for line in open(os.path.join(base_path(), "data", filename), "r"):
            if line.startswith('#'): continue

            values = line.split()
            if not values: continue

            if values[0] == 'newmtl':
                material = Material(values[1])
                self.materials[material.name] = material
            elif material is None:
                warnings.warn('Expected "newmtl" in %s' % filename)
                continue

            if values[0] == 'Kd':
                material.diffuse = Vec3(*map(float, values[1:]))
            elif values[0] == 'Ka':
                material.ambient = Vec3(*map(float, values[1:]))
            elif values[0] == 'Ks':
                material.specular = Vec3(*map(float, values[1:]))
            elif values[0] == 'Ke':
                material.emission = Vec3(*map(float, values[1:]))
            elif values[0] == 'Ns':
                material.shininess = float(values[1])
            elif values[0] == 'd':
                material.opacity = float(values[1])
            elif values[0] == 'map_Kd':
                material.texture = load_texture(values[1])
