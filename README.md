# php-wbootstrap

`php-wbootstrap` allow you to execute the good version of PHP according Wampserver configuration in _Windows_ system.
In general, if you use many version of [php](http://php.net/downloads.php) with Wampserver, you configure only one path to your _php.exe_ (i.e. `C:\wamp\bin\php\php7.0.10\`).
If you use another version of php, you have a conflict with *command prompt (cmd)* based on the path and php launched by Wampserver.

`php-wbootstrap` make easy the switch between each version of php through Wampserver.

# Environment

`php-wbootstrap` is built with **gcc** of [MinGW](http://www.mingw.org/) by the command:

```shell
gcc main.c
```

# Installation

1. Download the last release of this tool
2. Copy and rename `php-X.X.X.exe` to `C:\wamp\php.exe`
3. In **variable environment**, add the following path `C:\wamp\php`
4. Make sure that you don't have another path like `C:\wamp\bin\php\phpX.X.X\`

# Utilization

With you **shell command prompt**, execute `php`command with your parameters. Example:

```shell
php composer.phar update
```

This tool will display the _php version_ executed like:

```shell
C:\wamp\www\project>php composer.phar update
PHP Version: 7.0.10

...
```