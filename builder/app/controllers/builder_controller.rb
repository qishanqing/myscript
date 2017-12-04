# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')

class BuilderController < ApplicationController

  before_action :logged_in_user, only: [:index, :edit, :update, :destroy,
                                        :following, :followers]
  before_action :correct_user,   only: [:edit, :update]
  before_action :admin_user,     only: [:destroy]
  
  def index
  end
#  def mails_format_errors(extra_mails)
#	  extra_mails.split(' ').each { |mail|
#		  name = mail.sub(/@dafy.com/,'')
#		  if not name.match(/^[a-z0-9\.]+$/i) then
#			  return "#{mail} is not correct dafy email"
#		  end
#	  }
#	  return nil
 # end
  def new
	  @author = params[:author]
	  @types = params[:types]
	  @email = params[:email]
	  @extra_mails = params[:extra_mails]
	  @branch = params[:branch]
	  @riqi = params[:riqi]
	  @trunk = params[:trunk]
	  result = nil
	  if @email.blank? then
	    @error = "用户邮箱不能为空"
#          elsif params[:types] == "cl" or params[:types] == "cr" then
#            @trunk.blank = true and @branch.blank = true
#            render 'builder/create'
	  elsif @trunk.blank? or @branch.blank? then
		  @error = "相对主干路径、分支名不能为空"
#	  elsif mails_format_errors then
#		  @error = "通知邮件收件人格式不正确：" + mails_format_errors(extra_mails)
	  else
		  result = true
	  end

	  if not result then
		  render 'index'
	  else
		  system  "svn-branchcreate.sh", \
			  "-a", @author, \
			  "-t", @trunk, \
			  "-e", @email, \
			  "-E", @extra_mails, \
			  "-b", @branch, \
			  "-T", @types, \
			  "-d", @riqi
	  end
  end
end
