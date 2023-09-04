/bin/bash ./lib/junebug/scripts/unix/web-build.sh

html_file=$(find "./embuild" -maxdepth 1 -type f -name "*.html" -print -quit)

if [ -n "$html_file" ]; then
    # Extract the filename without the path
    html_filename=$(basename "$html_file")
    
    # Open the file in the browser
    emrun --port 8080 embuild/"$html_filename"
else
    emrun --port 8080 embuild
fi