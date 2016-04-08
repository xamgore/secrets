### Task

Make a web-page with an authorization form (login & password).
If password is valid, then show any secret about user, otherwise ban user by IP.
Only 3 attempts per minute are available.

### Compilation

For production:

```bash
$ cd app/
$ g++ main.cpp utils.h md5.h md5.cpp -std=c++11 -o ../public_html/index.cgi
```

For development (`inotify-tools` must be installed):

```
$ cd app/
$ while inotifywait -e close_write,moved_to,create ./main.cpp
    sleep 0.5
    g++ main.cpp utils.h md5.h md5.cpp -std=c++11 -o ../public/index.cgi
end
```

### Apache config

```xml
<VirtualHost *:80>
    <Directory "/home/ksa/Work/mmcs/net/secret">
        AddHandler cgi-script .py .cgi .php
        Options Indexes FollowSymLinks Includes ExecCGI
        AllowOverride All
        Order allow,deny
        Allow from all
    </Directory>

    ServerName secret.io
    DocumentRoot "/home/ksa/Work/mmcs/net/secret/public"
    ErrorLog "/home/ksa/Work/mmcs/net/secret/errors.log"
</VirtualHost>
```
