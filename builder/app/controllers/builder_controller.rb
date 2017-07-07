# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')

class BuilderController < ApplicationController
	http_basic_authenticate_with name: "admin", password: "qsq", except: [:index]
  def index
  end
  def new
	  user = params[:user]
	  types = params[:types]
	  email = params[:email]
	  extra_mails = params[:extra_mails]
	  branch = params[:branch]
	  riqi = params[:riqi]
	  trunk = params[:trunk]
	  
	  result = nil
	  if email.blank? then
		  @error = "用户邮箱不能为空"
	  elsif trunk.blank? or branch.blank? then
		  @error = "相对主干路径、分支名不能为空"
	  else
		  result = true
	  end

	  if not result then
		  render 'index'
	  else
		  system  "web-branchcreate.sh", \
			  "-u", user, \
			  "-t", trunk, \
			  "-e", email, \
			  "-E", extra_mails, \
			  "-b", branch, \
			  "-T", types, \
			  "-d", riqi
	  end
  end
end
