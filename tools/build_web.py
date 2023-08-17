import os

print('getcwd:      ', os.getcwd())

input_file = '.\\lib\\Web\\index.html'
output_file = ".\\lib\\Web\\Web.cpp"
string_start = "R\"(\n"
string_end = "\n)\""

header = "#include \"Web.h\"\n#include <Arduino.h>\n\nconst char Web_index[] PROGMEM = "


with open(os.getcwd() + input_file, 'r') as f:
    content = f.read()

output_content = f"{header}{string_start}{content}{string_end};"

with open(output_file, 'w') as f:
    f.write(output_content)

print(f"Conversion complete. Saved as {output_file}")
