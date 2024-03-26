import json
import re

def parse_build_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.read().strip().split('\n')
    
    build_config = []
    
    current_item = {}
    
    for line in lines:
        line = line.strip()
        
        # Detect section start
        if line.startswith("cc_binary("):
            if current_item:  
                build_config.append(current_item)
            current_item = {"type": "cc_binary"}
        elif line.startswith("cc_library("):
            if current_item:  
                build_config.append(current_item)
            current_item = {"type": "cc_library"}
        elif line == ")":
            if current_item:  
                build_config.append(current_item)
                current_item = {}
        else:
            key_value_match = re.match(r'(\w+)=\[(.*?)\]|(\w+)="(.*?)"', line)
            if key_value_match:
                key = key_value_match.group(1) or key_value_match.group(3)
                value = key_value_match.group(2) or key_value_match.group(4)
                
                if value.startswith(":"):
                    value = [value]  
                elif "," in value:
                    value = value.split(",")  
                    value = [v.strip().strip('"') for v in value]
                else:
                    value = value.strip('"')  
                    
                
                current_item[key] = value
    
    
    if current_item:
        build_config.append(current_item)
    
    return build_config

file_path = "foo.build"
parsed_build = parse_build_file(file_path)

with open("foo.json", "w") as json_file:
    json.dump(parsed_build, json_file, indent=4)

print("Build configuration saved to foo.json.")
