Rails.application.routes.draw do
  get 'merge/index'
  get 'articles/index'
  #resources :articles
  resources :builder
  resources :articles
  resources :merge

  match '/start',  to: 'builder#new',            via: 'post'
  match '/articles/submit',  to: 'articles#submit',            via: 'post'
  match '/merge/new',  to: 'merge#new',            via: 'post'

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
  root  'builder#index'
end
