# cent

A simple tool which reads data from a file (or stdin) and calculates its
entropy.

## Building / installing

    make [debug]
    make install

## Usage

	cent [-w|--whole] [-h|--help] [file]

	file        If parameter is missing, it will use stdin (if present) or just use /dev/urandom.
	-w|--whole  Read the whole file. If not set, just reads up to 4096 bytes.
	-h|--help   This screen.

## Examples

	head -c 255 /dev/urandom | cent
	openssl enc --aes-128-cbc -pbkdf2 -k test </etc/passwd | cent -w
	perl -e 'for(0..255){print chr}' | cent
	cent /etc/group

## License

Please see the [LICENSE](./LICENSE) file.

[//]: # ( vim: set tw=80: )
