#!/bin/bash

export PATH="$PATH:~/myscript/sh"

function get-job-info() {
    jc get-job $JOB_NAME > $template
}

function output-manifest.xml-from-template() {
    get-job-info
    c="<hudson.plugins.timestamper.TimestamperBuildWrapper plugin=timestamper@1.20/>\n  <org.jenkinsci.plugins.builduser.BuildUser plugin=build-user-vars-plugin@1.9/>"
    if ! `cat $template | grep "org.jenkinsci.plugins.builduser" `;then
	sed -i "s#hudson.plugins.timestamper.TimestamperBuildWrapper.*#$c#g" $template
    fi
}

function jc-update-job() {
    template="/root/tmp/logs/$JOB_NAME.xml"
    output-manifest.xml-from-template


    for n in $(seq 1 10);do
	cat $template | jc update-job $JOB_NAME >& ~/tmp/logs/output.$$ &&  break || true
	sleep 1
    done
}

for JOB_NAME in `jc list-jobs | grep .*sdk`;do
    jc-update-job
done
