## Description
dump denetry information

## Background
...

## Documentation

### insmod and dump
```shell
insmod ./ddump.ko && rmmod ./ddump.ko
```

output file is /tmp/dump_dentry.out :
```
>>> Start analyze "/" dentry
COUNT:1      /lib
COUNT:244      /etc
COUNT:1      /bin
COUNT:530      /var
COUNT:66      /tmp
COUNT:58      /opt
COUNT:19957      /usr
COUNT:1      /sbin
COUNT:1      /lib64
COUNT:1      /dump
COUNT:1      /test
COUNT:465      /root
COUNT:1      /META-INF
COUNT:1      /selinux
COUNT:1      /LogSetting.ini
<<< End analyze "/" dentry: 21329
```

### module parm desc
>注意：dump_depth不易过大，否则会导出数据过多导致磁盘空间爆满
```
dump_depth:int          default: 1
dump_directory:charp    default: "/"

dump_match_on:int	default: 0
dump_match_key:charp    default: "tmp"
dump_match_max:int      default: 200000
```


## Status
...


## Building

```shell
make
```


## Communication channels
...


## Contributing
...


## Licensing

[GPL v2.0 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
All contributions retain ownership by their original author, but must also
be licensed under the GPL 2.0 or later to be merged.
