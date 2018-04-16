# coding: utf-8
class DevHelpersController < ApplicationController
  skip_before_action :verify_authenticity_token, only: [:new]
  def index
  end

  def show
  end

  def new
    @types = params[:types]
    @extra_mails = params[:extra_mails]
    @email = params[:email]
    @branch = params[:branch]
    @message = params[:message]
    @branch_name = params[:branch_name]

    result = nil
    if @branch.blank? then
      @error = "分支名不能为空"
    else
      result = true
    end

    if not result then
      render 'index'
    else
      system  "svn-tag.sh", \
              "-E", @extra_mails, \
              "-e", @email, \
              "-b", @branch, \
              "-T", @types, \
              "-m", @message
    end
  end
end
