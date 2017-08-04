class DataFile < ApplicationController
	def self.save(upload)
		name = upload('uploadFile').original_filename
		directory = "/home/qishanqing/upload/public"

		path = File.join(directory, name)
		File.open(path, "wb") { |f| f.write(Upload['uploadFile'].read) }
	end
end
