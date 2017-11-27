# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class JenkinsController < ApplicationController

  before_action :logged_in_user, only: [:index, :edit, :update, :destroy,
                                        :following, :followers]
  before_action :correct_user,   only: [:edit, :update]
  before_action :admin_user,     only: [:destroy]
  
  def index
  end
  def new
	  add = params[:add]
	  del = params[:del]
	  copy = params[:copy]
	  email = params[:email]
	  extra_mails = params[:extra_mails]
	  num = params[:num]
	  types = params[:types]
	  
	  result = nil

	  if email.blank? then
		  @error = "用户邮箱不能为空"
	  elsif types == 'add' and add.blank? then
		  @error = "新增项目名称不能为空"
	  elsif types == 'del' and del.blank? then
		  @error = "删除项目名称不能为空"
	  else
		  result = true
	  end


	  if not result then
		  render 'index'
	  elsif not types.blank? and not num.blank? then
		  system  "jenkins.sh", \
			  "-a", add, \
			  "-d", del, \
			  "-T", types, \
			  "-n", num, \
			  "-c", copy, \
			  "-e", email, \
			  "-E", extra_mails
	  elsif not types.blank? then
		  system  "jenkins.sh", \
			  "-a", add, \
			  "-d", del, \
			  "-T", types, \
			  "-c", copy, \
			  "-e", email, \
			  "-E", extra_mails
	  else
		  system  "jenkins.sh", \
			  "-a", add, \
			  "-d", del, \
			  "-n", num, \
			  "-c", copy, \
			  "-e", email, \
			  "-E", extra_mails
	  end
  end
end
