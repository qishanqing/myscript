Rails.application.routes.draw do
  #get 'builder/index'
  get 'articles/new'
  #resources :articles
  resources :builder
  match '/start',  to: 'builder#new',            via: 'post'

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
  root 'builder#index'
end
