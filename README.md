## Description
dump denetry information

## Background

## Documentation

insmod and dump
```shell
insmod ./ddump.ko && rmmod ./ddump.ko
```

output file is /tmp/dump_dentry.out
```
>>> start analyze "/" dentry
~/ Count: 1
lib/ Count: 1
bin/ Count: 1
sbin/ Count: 1
lib64/ Count: 1
rpc/etc/ Count: 1
lib/var/ Count: 1
pki/etc/ Count: 4
sda/mnt/ Count: 1
etc/ssh/ Count: 7
log/var/ Count: 10
etc/rpm/ Count: 6
lib/usr/ Count: 1158
etc/gss/ Count: 2
...
redhat-release/etc/ Count: 1
initrd-release/etc/ Count: 1
anaconda-ks.cfg/root/ Count: 1
dump_dentry.out/tmp/ Count: 1
resolv.conf.save/etc/ Count: 1
<<< end analyze "/" dentry: 28542917
```

## Status

## Building

```shell
make
```

## Communication channels


## Contributing


## Licensing

[GPL v2.0 or later](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
All contributions retain ownership by their original author, but must also
be licensed under the GPL 2.0 or later to be merged.