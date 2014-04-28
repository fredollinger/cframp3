#
# Regular cron jobs for the cframp3 package
#
0 4	* * *	root	[ -x /usr/bin/cframp3_maintenance ] && /usr/bin/cframp3_maintenance
