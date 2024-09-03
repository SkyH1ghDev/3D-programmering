import xml.etree.ElementTree as ET
import os
import sys

def add_include_directory(vcxproj_path: str, new_include_dirs: list):
    
    namespace: str = "http://schemas.microsoft.com/developer/msbuild/2003"
    prefixNamespace: str = "{" + namespace + "}"
    
    ET.register_namespace('', namespace)
    
    tree = ET.parse(vcxproj_path)
    root = tree.getroot()
    
    for item in root.iter(prefixNamespace + 'ItemDefinitionGroup'):
        for clCompile in item.iter(prefixNamespace + 'ClCompile'):
            for prop in clCompile.iter(prefixNamespace + 'AdditionalIncludeDirectories'):
                prop.text = '\n\t\t'
                
                for include_dir in new_include_dirs:
                    prop.text = prop.text + '; \n\t\t' + include_dir if prop.text != "\n\t\t" else prop.text + include_dir
                    
                prop.text = prop.text + '\n' + ' ' * 6
                
    for item in root.iter(prefixNamespace + 'ItemGroup'):
        for clCompile in item.iter(prefixNamespace + 'ClCompile'):
            for prop in clCompile.iter(prefixNamespace + 'AdditionalIncludeDirectories'):
                prop.text = '\n\t\t'
                
                for include_dir in new_include_dirs:
                    prop.text = prop.text + '; \n\t\t' + include_dir if prop.text != "\n\t\t" else prop.text + include_dir
                    
                prop.text = prop.text + '\n' + ' ' * 6
                
    tree.write(vcxproj_path)



def get_all_file_paths(root_dir: str):
    all_file_paths: list = [root_dir]
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for dirname in dirnames:
            all_file_paths.append(os.path.join(dirpath, dirname))
            
    for i in range(0, len(all_file_paths)):
        all_file_paths[i] = all_file_paths[i].replace('\\', '/')
        all_file_paths[i] = all_file_paths[i].removeprefix(project_path)
        
    all_file_paths.remove("Src/Shaders")
    all_file_paths.append("Resources/Libraries")
    
    return all_file_paths
   
   

if __name__ == '__main__':
    
    project_path: str = sys.argv[1]
    #project_path = project_path[:-1]
    project_path = project_path.replace('\\', '/')
    add_include_directory(project_path + "3D Projekt.vcxproj", get_all_file_paths(project_path + "Src"))