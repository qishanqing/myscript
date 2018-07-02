# coding: utf-8
class User < ActiveRecord::Base
  has_many :microposts, dependent: :destroy
  has_many :active_relationships, class_name:  "Relationship",
                                  foreign_key: "follower_id",
                                  dependent:   :destroy  
  has_many :passive_relationships, class_name:  "Relationship",
                                   foreign_key: "followed_id",
                                   dependent:   :destroy                               
  has_many :following, through: :active_relationships,  source: :followed
  has_many :followers, through: :passive_relationships, source: :follower

  attr_accessor :remember_token, :activation_token, :reset_token
  before_save { self.email = email.downcase }
  before_create :create_activation_digest
  
  validates :name,  presence: true, length: { maximum: 50 }
  #VALID_EMAIL_REGEX = /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/i
  VALID_EMAIL_REGEX = /\A[\w+\-.]+@dafy.com/i
  validates :email, presence: true, length: { maximum: 255 },
                    format: { with: VALID_EMAIL_REGEX },
                    uniqueness: { case_sensitive: false },
                    inclusion: { in: ['hanyunfeng@dafy.com','qishanqing@dafy.com','yujigang@dafy.com','wangyang02@dafy.com','liuzijie@dafy.com','liusen@dafy.com','yangguang@dafy.com','weishaolong@dafy.com','pankun@dafy.com','xiaojian@dafy.com','zhangjunjie@dafy.com','jinshaofei@dafy.com','jinagzhenhui@dafy.com','yanghaizhu@dafy.com','lihaihan@dafy.com','liuxuxing@dafy.com','xizhixing@dafy.com','songxinyu@dafy.com','tangchuanjin@dafy.com','lideyao@dafy.com','leiwanda@dafy.com','heweiming@dafy.com','yanhai@dafy.com','muxiaoyu@dafy.com','guojinlong@dafy.com','wangrongjiu@dafy.com','chenhongan@dafy.com','niutieliang@dafy.com','qiyongliang@dafy.com','zhangjiawen@dafy.com','renshibo@dafy.com','chenxiguang@dafy.com'], message:'你没有权限注册'}
  has_secure_password
  validates :password, presence: true, length: { minimum: 6 }, allow_nil: true

  # Returns the hash digest of the given string.
  def User.digest(string)
    cost = ActiveModel::SecurePassword.min_cost ? BCrypt::Engine::MIN_COST :
                                                  BCrypt::Engine.cost
    BCrypt::Password.create(string, cost: cost)
  end

  # Returns a random token.
  def User.new_token
    SecureRandom.urlsafe_base64
  end

  # Remembers a user in the database for use in persistent sessions.
  def remember
    self.remember_token = User.new_token
    update_attribute(:remember_digest, User.digest(remember_token))
  end

  # Returns true if the given token matches the digest.
  def authenticated?(attribute, token)
    digest = send("#{attribute}_digest")
    return false if digest.nil?
    BCrypt::Password.new(digest).is_password?(token)
  end

  # Forgets a user.
  def forget
    update_attribute(:remember_digest, nil)
  end

  # Activates an account.
  def activate
    update_attribute(:activated,    true)
    update_attribute(:activated_at, Time.zone.now)
  end

  # Sends activation email.
  def send_activation_email
    UserMailer.account_activation(self).deliver_now
  end

  # Sets the password reset attributes.
  def create_reset_digest
    self.reset_token = User.new_token
    update_attribute(:reset_digest,  User.digest(reset_token))
    update_attribute(:reset_sent_at, Time.zone.now)
  end

  # Sends password reset email.
  def send_password_reset_email
    UserMailer.password_reset(self).deliver_now
  end

  # Returns true if a password reset has expired.
  def password_reset_expired?
    reset_sent_at < 2.hours.ago
  end

   # Returns a user's status feed.
  def feed
    following_ids = "SELECT followed_id FROM relationships
                     WHERE  follower_id = :user_id"
    Micropost.where("user_id IN (#{following_ids})
                     OR user_id = :user_id", user_id: id)
  end

  # Follows a user.
  def follow(other_user)
    active_relationships.create(followed_id: other_user.id)
  end

  # Unfollows a user.
  def unfollow(other_user)
    active_relationships.find_by(followed_id: other_user.id).destroy
  end

  # Returns true if the current user is following the other user.
  def following?(other_user)
    following.include?(other_user)
  end
  
  private
    # Converts email to all lower-case.
    def downcase_email
      self.email = email.downcase
    end

    # Creates and assigns the activation token and digest.
    def create_activation_digest
      self.activation_token  = User.new_token
      self.activation_digest = User.digest(activation_token)
    end

end
