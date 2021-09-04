import os
import shutil


def to_stuff(format, path):
    if len(path) == 0:
        return ""

    return "\t\t{}\n".format(format.format(path))


def main():
    controller_name = input("Controller Name: ")
    sdk_path = input("SDK Path (relative to ThirdParty directory): ")
    path_to_include = input("Include Path (relative to SDK path): ")
    path_to_dll = input("DLL Path (relative to SDK path, can be empty): ")
    path_to_lib = input("Lib Path (relative to SDK path, can be empty): ")

    include = to_stuff("PublicIncludePaths.Add(Path.Combine(SDKDir, \"{}\"));", path_to_include)
    dll = to_stuff("RuntimeDependencies.Add(Path.Combine(SDKDir, \"{}\"));", path_to_dll)
    lib = to_stuff("PublicAdditionalLibraries.Add(Path.Combine(SDKDir, \"{}\"));", path_to_lib)
    delay_load = to_stuff("PublicDelayLoadDLLs.Add(\"{}\");", os.path.basename(path_to_dll))

    should_execute = input("Create Controller (y/n):").lower()

    if should_execute != "y":
        print("Didn't create controller")
        return

    controller_base_dir = "../Source/Controllers/{}".format(controller_name)

    def copy_rename(src, dst, *, follow_symlinks=True):
        filename = controller_name.join(dst.rsplit("Template", 1))
        shutil.copy2(src, filename, follow_symlinks=follow_symlinks)

    shutil.copytree("ControllerTemplate/Template", controller_base_dir,
                    copy_function=copy_rename, dirs_exist_ok=True)

    def read_content(filename):
        with open(filename, "r") as file:
            return file.readlines()

    def write_content(filename, content):
        with open(filename, "w") as file:
            new_content = []
            for x in content:
                x = x.replace("Template", controller_name)
                x = x.replace("zzzSDKPathName", sdk_path)
                x = x.replace("zzzIncludePath", include)
                x = x.replace("zzzDllPath", dll)
                x = x.replace("zzzLibPath", lib)
                x = x.replace("zzzDelayLoadDll", delay_load)
                new_content.append(x)

            file.writelines(new_content)

    for filename in os.listdir(controller_base_dir):
        filename = os.path.join(controller_base_dir, filename)
        content = read_content(filename)
        write_content(filename, content)

    device_rgb_build = "../Source/DeviceRGB/DeviceRGB.Build.cs"
    with open(device_rgb_build, "r") as file:
        build = file.readlines()

    index_to_insert_at = [x.strip() for x in build].index("// Add new controllers here!")
    build.insert(
        index_to_insert_at, "\t\t\tTryAddController(\"{}\", \"{}Controller\");\n".format(
            sdk_path, controller_name))

    with open(device_rgb_build, "w") as file:
        file.writelines(build)


if __name__ == "__main__":
    main()
