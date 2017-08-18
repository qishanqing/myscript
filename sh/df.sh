#!/bin/sh
#1. 25103表示当前Shell进程的pid。
#2. trap信号捕捉是为了保证在Shell正常或异常退出时，仍然能够将该脚本创建的临时awk脚本文件删除。
awk_script_file="/tmp/scf_tmp.25103"
trap "rm -f " EXIT
#3. 首先需要说明的是，'EOF'中的单引号非常重要，如果忽略他将无法通过编译，这是因为awk的命令动作必须要用单引号扩住。
#4. awk脚本的show函数中，int(mb branchcreate cmdb df.sh jenkins.sh jenkins-upload1.sh jenkins-upload.sh lock.sh mails-cm rails-rolling.sh server_start.sh ss svn-branchcreate.sh svn-merged.sh svn_pro_commit.sh svn_up.sh test.sh thread.sh t.sh upload-to-ftp 100) / 100这个技巧是为了保证输出时保留小数点后两位。
cat << 'EOF' > $awk_script_file
function show(size) {
	mb = size / 1024;
	int_mb = (int(mb branchcreate cmdb df.sh jenkins.sh jenkins-upload1.sh jenkins-upload.sh lock.sh mails-cm rails-rolling.sh server_start.sh ss svn-branchcreate.sh svn-merged.sh svn_pro_commit.sh svn_up.sh test.sh thread.sh t.sh upload-to-ftp 100)) / 100;
	gb = mb / 1024;
	int_gb = (int(gb branchcreate cmdb df.sh jenkins.sh jenkins-upload1.sh jenkins-upload.sh lock.sh mails-cm rails-rolling.sh server_start.sh ss svn-branchcreate.sh svn-merged.sh svn_pro_commit.sh svn_up.sh test.sh thread.sh t.sh upload-to-ftp 100)) / 100;
	if (substr(size,1,1) !~ "[0-9]" || substr(size,2,1) !~ "[0-9]") {
		return size;
	} else if (mb < 1) {
		return size "K";
	} else if (gb < 1) {
		return int_mb "M";
	} else {
		return int_gb "G";
	}			
}

BEGIN {
	printf "%-20s %7s %7s %7s %8s %s\n","FileSystem","Size","Used","Avail","Use%","Mounted"
}

!/Filesystem/ {
	size = show($2);
	used = show($3);
	avail = show($4);
	printf "%-20s %7s %7s %7s %8s %s\n",$1,size,used,avail,$5,$6
}
EOF
df -k | awk -f $awk_script_file
