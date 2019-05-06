# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class MergeController < ApplicationController
  skip_before_action :verify_authenticity_token, only: [:new]

  before_action :logged_in_user, only: [:index, :edit, :update, :destroy,
                                        :following, :followers]
  before_action :correct_user,   only: [:edit, :update]
  before_action :admin_user,     only: [:destroy]
  
  def index
  end
  def new
	  @task = params[:task]
	  @rev = params[:rev]
	  @types = params[:types]
	  @extra_mails = params[:extra_mails]
	  @branch = params[:branch]
	  @trunk = params[:trunk]
	  @owner = current_user.email
          
	  result = nil
	  if @branch.blank? then
		  @error = "相对主干路径、分支名不能为空"
	  else
		  result = true
	  end

	  if not result then
		  render 'index'
	  else
		  system  "svn_merged", \
			  "-k", @task, \
			  "-o", @owner, \
			  "-R", @rev, \
			  "-t", @trunk, \
			  "-E", @extra_mails, \
			  "-b", @branch, \
			  "-T", @types
	 end
  end
end
