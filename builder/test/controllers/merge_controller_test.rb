require 'test_helper'

class MergeControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get merge_index_url
    assert_response :success
  end

end
