# coding: utf-8
require 'rubygems'

$LOAD_PATH << ( Rails.root + 'app/controllers')
class UploadController < ApplicationController
  def index
	  @uploadfile = params[:uploadFile]
  end
  def create
	  @upload = Upload.new(uploadFile)
	  unless request.get?
		  puts "*********************************************************"
		  puts filename
		  @product.image_url = filename
		  @product.save
	  end
  end
  protected
  def uploadfile(file)
	   if !file.original_filename.empty?
		   @filename = file.original_filename
		   FileUtils.mkdir("#/home/qishanqing/upload/public") unless File.exist?("#/home/qishanqing/upload/public")
		   File.open("#/home/qishanqing/upload/public/#{@filename}","wb") do |f|
			   f.write(file.read)
		   end
		   return @filename
	   end
  end
  def product_params
	  params.require(:uploadFile).permit(:image_url)
  end
end
#  def uploadFile
#	  require 'fileutils'
#	  tmp = params[:upload][:uploadFile]
#	  file = File.join("/home/qishanqing/upload/public", tmp.original_filename)
#	  FileUtils.cp tmp.path, file
#  end

