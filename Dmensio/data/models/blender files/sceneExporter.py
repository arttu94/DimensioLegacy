bl_info = { "name": "export Dmensio Scene",
"location": "File > Import-Export",
"category": "Import-Export",
}

import bpy

class ExportDMscene(bpy.types.Operator):
    bl_label = "export custom scene"
    bl_idname = "object.exportdm"
    bl_options = {'PRESET'}
    
    def execute(self, context):
        
        import os
        file = open(os.path.splitext(bpy.data.filepath)[0] + ".txt", 'w')
        
        for obj in bpy.data.objects:
            
            if("Cube" in obj.name):
                file.write("%s p \n %f  %f  %f \n s %f  %f  %f \n" % (obj.name, obj.location.x, obj.location.z, obj.location.y, obj.scale.x, obj.scale.z, obj.scale.y))
                
                file.close()
                
                return {'FINISHED'}

def menu_func(self, context):
    self.layout.operator(ExportDMscene.bl_idname, text="DM txt(.txt)")
            
def register():
    bpy.utils.register_class(ExportDMscene)
                   
def unregister(): 
    bpy.utils.unregister_class(ExportDMscene)              


if __name__ == "__main__":
    register()                       