#!/usr/bin/python

f = open("index.html")

html = ""
for line in f:
    html += line.replace('"','\\"').replace('\n','\\\n')

output = open("../html.h", "w")
output.write('char main_html[] = "'+html+'";\n')
output.write('unsigned size_main_html = '+str(len(html))+';\n')
output.close()
print("char length:", len(html));
