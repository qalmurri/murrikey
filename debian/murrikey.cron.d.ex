#
# Regular cron jobs for the murrikey package.
#
0 4	* * *	root	[ -x /usr/bin/murrikey_maintenance ] && /usr/bin/murrikey_maintenance
