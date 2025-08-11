import re

def get_white_list():
    """
    返回允许提取的 struct 名称白名单列表
    """
    list = ["RcimMessageBox" , "RcimPushConfig", "RcimAndroidConfig", "RcimHarmonyConfig",
        "RcimIosConfig", "RcimReadReceiptInfo", "RcimReadReceiptInfoV2", "RcimReadReceiptUserInfo",
        "RcimReceivedStatus", "RcimEngineBuilderParam", "RcimSDKVersion", "RcimReceivedInfo"]
    return list  # 你可以往这里添加更多结构体名称

def extract_structs(header_content):
    """
    用正则提取所有 struct 定义（支持 typedef struct 和 struct Name { ... } Name）
    返回提取到的所有结构体文本和对应结构体名称的列表元组
    """
    pattern = re.compile(
        r"(typedef\s+)?struct\s+(\w*)\s*{[^{}]*?(?:{[^{}]*}[^{}]*?)*}\s*(\w*)\s*;",
        re.DOTALL | re.MULTILINE
    )
    results = []
    for match in pattern.finditer(header_content):
        full_struct_text = match.group(0)
        # 结构体名称有两种可能：struct 后的名字(match.group(2)) 或 typedef 后面的名字(match.group(3))
        struct_name = match.group(3) or match.group(2)
        results.append((full_struct_text, struct_name))
    return results

def do_extract_structs(input_path, output_path):
    with open(input_path, "r", encoding="utf-8") as f:
        content = f.read()

    extracted = extract_structs(content)
    white_list = get_white_list()
    filtered_structs = []

    for struct_text, struct_name in extracted:
        if struct_name in white_list:
            # 去掉 enum 关键字，方便识别
            struct_text = struct_text.replace("enum ", "")
            filtered_structs.append(struct_text)
        else:
            print(f"【警告】结构体 '{struct_name}' 不在白名单中，未提取")

    with open(output_path, "w", encoding="utf-8") as f:
        for struct_text in filtered_structs:
            f.write(struct_text.strip() + "\n\n")

    print(f"提取完成，共 {len(filtered_structs)} 个 struct，已写入 {output_path}")

def main():
    input_path = "imlibV2/src/main/cpp/libs/includes/rcim_client.h"
    output_path = "imlibV2/src/main/cpp/swig/extracted_struct_define.h"
    do_extract_structs(input_path, output_path)

if __name__ == "__main__":
    main()
