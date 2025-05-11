# Convert the update page to a C header file

### go to the web directory
```bash
cd web
```

### compress the update page
```bash
gzip -c update.html > UPDATE_PAGE
```

### convert to c header
```bash
xxd -i UPDATE_PAGE > ../src/UPDATE_PAGE.h
```