import json

with open("../Levels/level1.json") as f:
    data = json.load(f)

circles = data["circles"]

with open("../src/Source/LevelData.cpp", "w") as cpp:
    cpp.write('#include "../Header/LevelData.h"\n\n')
    cpp.write('const CircleLevelData level1[] = {\n')
    for c in circles:
        id_str = f'"{c["id"]}"'
        parent_str = f'"{c["parentId"]}"' if "parentId" in c else 'nullptr'
        tags = c.get("tags", [])
        if tags:
            tags_str = "{" + ", ".join(f'"{tag}"' for tag in tags) + "}"
        else:
            tags_str = "{}"
        cpp.write(f'    {{ {id_str}, {parent_str}, {c["x"]}, {c["y"]}, {c["radius"]}, std::vector<std::string>{tags_str} }},\n')
    cpp.write("};\n")
    cpp.write(f"const size_t level1Size = {len(circles)};\n")

with open("../src/Header/LevelData.h", "w") as h:
    h.write("#pragma once\n")
    h.write("#include <cstddef>\n")
    h.write("#include <vector>\n")
    h.write("#include <string>\n\n")
    h.write("struct CircleLevelData {\n")
    h.write("    const char* id;\n")
    h.write("    const char* parentId;\n")
    h.write("    float x;\n")
    h.write("    float y;\n")
    h.write("    float radius;\n")
    h.write("    std::vector<std::string> tags;\n")
    h.write("};\n\n")
    h.write("extern const CircleLevelData level1[];\n")
    h.write("extern const size_t level1Size;\n")