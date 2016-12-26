#
# Regular cron jobs for the newlib package
#
0 4	* * *	root	[ -x /usr/bin/newlib_maintenance ] && /usr/bin/newlib_maintenance
