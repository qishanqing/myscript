<?xml version='1.0' encoding='UTF-8'?>
<maven2-moduleset plugin="maven-plugin@2.17">
  <actions/>
  <description>Jenkins Auto Web Create&#xd;
&#xd;
branch: https://192.168.0.220/svn/tech/Branch/20180606-苏宁订单退货流程优化/Develop/分期商城/分期商城3.0/MallRefundBusiness&#xd;
&#xd;
creator:&#xd;
&#xd;
update_time: 2018-06-07-13.42.57</description>
  <keepDependencies>false</keepDependencies>
  <properties>
    <jenkins.model.BuildDiscarderProperty>
      <strategy class="hudson.tasks.LogRotator">
        <daysToKeep>-1</daysToKeep>
        <numToKeep>5</numToKeep>
        <artifactDaysToKeep>-1</artifactDaysToKeep>
        <artifactNumToKeep>-1</artifactNumToKeep>
      </strategy>
    </jenkins.model.BuildDiscarderProperty>
  </properties>
  <scm class="hudson.scm.SubversionSCM" plugin="subversion@2.9">
    <locations>
      <hudson.scm.SubversionSCM_-ModuleLocation>
        <remote>https://192.168.0.220/svn/tech/Branch/20180606-苏宁订单退货流程优化/Develop/分期商城/分期商城3.0/MallRefundBusiness</remote>
        <credentialsId>b5ef5c85-6a9d-4cf8-89bc-fa16ff19efd4</credentialsId>
        <local>.</local>
        <depthOption>infinity</depthOption>
        <ignoreExternalsOption>true</ignoreExternalsOption>
      </hudson.scm.SubversionSCM_-ModuleLocation>
    </locations>
    <excludedRegions></excludedRegions>
    <includedRegions></includedRegions>
    <excludedUsers></excludedUsers>
    <excludedRevprop></excludedRevprop>
    <excludedCommitMessages></excludedCommitMessages>
    <workspaceUpdater class="hudson.scm.subversion.CheckoutUpdater"/>
    <ignoreDirPropChanges>false</ignoreDirPropChanges>
    <filterChangelog>false</filterChangelog>
  </scm>
  <assignedNode>build2</assignedNode>
  <canRoam>false</canRoam>
  <disabled>false</disabled>
  <blockBuildWhenDownstreamBuilding>false</blockBuildWhenDownstreamBuilding>
  <blockBuildWhenUpstreamBuilding>false</blockBuildWhenUpstreamBuilding>
  <triggers/>
  <concurrentBuild>false</concurrentBuild>
  <rootModule>
    <groupId>com.dafy.mall</groupId>
    <artifactId>MallRefundBusiness</artifactId>
  </rootModule>
  <goals>clean -Pprod package  -U -Dmaven.test.skip=true</goals>
  <aggregatorStyleBuild>true</aggregatorStyleBuild>
  <incrementalBuild>false</incrementalBuild>
  <ignoreUpstremChanges>false</ignoreUpstremChanges>
  <ignoreUnsuccessfulUpstreams>false</ignoreUnsuccessfulUpstreams>
  <archivingDisabled>false</archivingDisabled>
  <siteArchivingDisabled>false</siteArchivingDisabled>
  <fingerprintingDisabled>false</fingerprintingDisabled>
  <resolveDependencies>false</resolveDependencies>
  <processPlugins>false</processPlugins>
  <mavenValidationLevel>-1</mavenValidationLevel>
  <runHeadless>false</runHeadless>
  <disableTriggerDownstreamProjects>false</disableTriggerDownstreamProjects>
  <blockTriggerWhenBuilding>true</blockTriggerWhenBuilding>
  <settings class="jenkins.mvn.DefaultSettingsProvider"/>
  <globalSettings class="jenkins.mvn.DefaultGlobalSettingsProvider"/>
  <reporters/>
  <publishers/>
  <buildWrappers>
    <org.jenkinsci.plugins.builduser.BuildUser plugin="build-user-vars-plugin@1.5"/>
  </buildWrappers>
  <prebuilders>
    <hudson.tasks.Shell>
      <command>ssh qishanqing@192.168.0.231 &lt;&lt; !
pre-svnmerge -b $SVN_URL -T add -E qishanqing@dafy.com
!</command>
    </hudson.tasks.Shell>
  </prebuilders>
  <postbuilders>
    <hudson.tasks.Shell>
      <command>source /home/qishanqing/myscript/sh/jenkins-upload.sh</command>
    </hudson.tasks.Shell>
  </postbuilders>
  <runPostStepsIfResult>
    <name>FAILURE</name>
    <ordinal>2</ordinal>
    <color>RED</color>
    <completeBuild>true</completeBuild>
  </runPostStepsIfResult>
</maven2-moduleset>