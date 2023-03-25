# cent

A simple tool which reads up to 4096 bytes of data from a file (or stdin) and
calculates its entropy.

## Building / installing

    make [debug]
    make install

## Usage

	cent [file|-h|--help]

If parameter `file` is missing, it will use stdin (if present), or just use
`/dev/urandom`.

## Examples

	head -c 255 /dev/urandom | cent
	openssl enc --aes-256-cfb -pbkdf2 -k test </etc/passwd | head -c 255 | cent
	perl -e 'for(0..255){print chr($_)}' | cent
	cent /etc/group

## License

Please see the [LICENSE](./LICENSE) file.

[//]: # ( vim: set tw=80: )
