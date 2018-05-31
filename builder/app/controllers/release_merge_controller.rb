# coding: utf-8
class ReleaseMergeController < ApplicationController
  skip_before_action :verify_authenticity_token, only: [:new]

  before_action :logged_in_user, only: [:index, :edit, :update, :destroy,
                                        :following, :followers]
  before_action :correct_user,   only: [:edit, :update]
  before_action :admin_user,     only: [:destroy]

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
    @files = params[:files]
    @owner = current_user.email

    result = nil
    if @branch.blank? or @files.blank? then
      @error = "预上线合并分支名、冲突文件不能为空"
    else
      result = true
    end

    if not result then
      render 'index'
    else
      system  "release-merge.sh", \
              "-E", @extra_mails, \
              "-e", @email, \
              "-b", @branch, \
              "-f", @files, \
              "-T", @types, \
              "-o", @owner, \
              "-m", @message
    end
  end
end
