Rails.application.routes.draw do
  get 'password_resets/new'
  get 'password_resets/edit'
  get 'articles/index'
  get 'upload/index'


  get    'help'   => 'static_pages#help'
  get    'about'   => 'static_pages#about'
  get    'contact' => 'static_pages#contact'
  get    'signup'  => 'users#new'
  get     'jenkins' => 'jenkins#index'
  get     'builder' => 'builder#index'
  get     'merge' => 'merge#index'
  get    'login'   => 'sessions#new'
  post   'login'   => 'sessions#create'
  delete 'logout'  => 'sessions#destroy'
  get 'logout'  => 'sessions#destroy'
  delete 'microposts/:id'  => 'microposts#destroy'
  get 'microposts/:id'  => 'microposts#destroy'
  
  resources :users do
    member do
      get :following, :followers
    end
  end

  resources :users
  resources :account_activations, only: [:edit]
  resources :password_resets,     only: [:new, :create, :edit, :update]
  resources :microposts,          only: [:create, :destroy]
  resources :relationships,       only: [:create, :destroy]

#  resources :builder
  resources :articles
#  resources :merge
#  resources :jenkins
  resources :upload

  match '/start',  to: 'builder#new',            via: 'post'
  match '/articles/submit',  to: 'articles#submit',            via: 'post'
  match '/merge/new',  to: 'merge#new',            via: 'post'
  match '/jenkins/new',  to: 'jenkins#new',            via: 'post'
  match '/upload/new', to: 'upload#new', via: 'post'

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
  root  'static_pages#home'
end
