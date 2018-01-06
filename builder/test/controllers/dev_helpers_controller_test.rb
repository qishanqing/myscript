require 'test_helper'

class DevHelpersControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get dev_helpers_index_url
    assert_response :success
  end

  test "should get new" do
    get dev_helpers_new_url
    assert_response :success
  end

end
