require 'test_helper'

class ReleaseMergeControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get release_merge_index_url
    assert_response :success
  end

  test "should get new" do
    get release_merge_new_url
    assert_response :success
  end

end
