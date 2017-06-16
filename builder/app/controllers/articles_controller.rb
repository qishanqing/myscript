class ArticlesController < ApplicationController
	def index

	end

	def new
		user_email = params[:user_email]
		user_password = params[:user_password]

		result = nil
		if user_email.blank? or user_password.blank? then
			@error = "用户名、密码不能为空"
		else
			result = true
		end


		if not result then
			render 'index'
		else
			render 'create'
		end
	end
	
	def create
	end
end
