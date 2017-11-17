class UserMailer < ApplicationMailer

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.user_mailer.account_activation.subject
  #

  def account_activation(user)
    @user = user
    
    mail(to: @user.email, subject: "Account activation")
  end

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.user_mailer.password_reset.subject
  #
  def password_reset(user)
    @user = user

    mail to: user.email, subject: "Password reset"
  end
  def send_mail(params = {})
    @url  = 'http://192.168.0.232:3000/login'
    mail( :subject => 'test',
          :to => "qishanqing@dafy.com",
          :from => 'qishanqing@dafy.com',
          :date => Time.now
        )
  end
end
