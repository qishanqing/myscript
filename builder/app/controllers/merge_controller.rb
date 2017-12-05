# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class MergeController < ApplicationController

  before_action :logged_in_user, only: [:index, :edit, :update, :destroy,
                                        :following, :followers]
  before_action :correct_user,   only: [:edit, :update]
  before_action :admin_user,     only: [:destroy]
  
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
          owner = current_user.email
	  
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
			  "-T", types, \
                          "-o", owner
	 end
  end
end
