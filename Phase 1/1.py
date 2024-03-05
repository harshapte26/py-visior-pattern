import json
import re

def parse_build_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    build_config = {
        "cc_binary": {},
        "cc_library": []
    }
    
    current_section = None
    current_library = {}
    
    for line in lines:
        line = line.strip()
        
        # Detect section start
        if line.startswith("cc_binary("):
            current_section = "cc_binary"
        elif line.startswith("cc_library("):
            current_section = "cc_library"
            current_library = {}
        elif line == ")":
            if current_section == "cc_library":
                build_config[current_section].append(current_library)
                current_library = {}
            current_section = None
        else:
            # Parse key-value pairs
            key_value_match = re.match(r'(\w+)=\[(.*?)\]|(\w+)="(.*?)"', line)
            if key_value_match:
                key = key_value_match.group(1) or key_value_match.group(3)
                value = key_value_match.group(2) or key_value_match.group(4)
                
                if value.startswith(":"):
                    value = [value]  # Dependency case
                elif "," in value:
                    value = value.split(",")  # List case
                    value = [v.strip() for v in value]
                else:
                    value = value.strip('"')
                    
                if current_section == "cc_library":
                    current_library[key] = value
                else:
                    build_config[current_section][key] = value
    
    return build_config

# Example usage
file_path = "foo.build"  # Your build file path here
parsed_build = parse_build_file(file_path)

# Save the output to foo.json
with open("foo.json", "w") as json_file:
    json.dump(parsed_build, json_file, indent=4)
