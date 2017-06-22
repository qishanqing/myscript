# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class MergeController < ApplicationController
	http_basic_authenticate_with name: "admin", password: "123456", except: [:index]
  def index
  end
  def new
	  task = params[:task]
	  rev = params[:rev]
	  types = params[:types]
	  email = params[:email]
	  extra_mails = params[:extra_mails]
	  branch = params[:branch]
	  trunk = params[:trunk]
	  
	  result = nil
	  if email.blank? then
		  @error = "用户邮箱不能为空"
	  elsif branch.blank? then
		  @error = "相对主干路径、分支名不能为空"
	  else
		  result = true
	  end

	  if not result then
		  render 'index'
	  else
		  system  "svn-merged.sh", \
			  "-k", task, \
			  "-R", rev, \
			  "-t", trunk, \
			  "-e", email, \
			  "-E", extra_mails, \
			  "-b", branch, \
			  "-T", types
	 end
  end
end
