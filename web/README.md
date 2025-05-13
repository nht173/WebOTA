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

> If you're using Windows instead of Linux or macOS, it's recommended to use Git Bash to execute the above commands.

> If you don't have Git Bash, you can use [GnuWin](https://sourceforge.net/projects/gnuwin32/) to install gzip and manually install [xxd](https://sourceforge.net/projects/xxd-for-windows/).

> Please check if the PATH environment variable is set correctly