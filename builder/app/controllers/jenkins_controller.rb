# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class JenkinsController < ApplicationController
	http_basic_authenticate_with name: "admin", password: "qsq", except: [:index]
  def index
  end
  def new
	  add = params[:add]
	  del = params[:del]
	  copy = params[:copy]
	  email = params[:email]
	  extra_mails = params[:extra_mails]
	  
	  result = nil

	  if params[:types] == ""; then
		 types = ""
	  elsif params[:num] == ""; then
		  num = ""
	  end

 				  
	  if email.blank? then
		  @error = "用户邮箱不能为空"
	  else
		  result = true
	  end


	  if not result then
		  render 'index'
	  else
		  system  "jenkins.sh", \
			  "-T",types, \
			  "-a", add, \
			  "-d", del, \
			  "-n", num, \
			  "-c", copy, \
			  "-e", email, \
			  "-E", extra_mails
	 end
  end
end
