Rails.application.routes.draw do
  get 'merge/index'
  get 'articles/index'
  get 'jenkins/index'
  #resources :articles
  resources :builder
  resources :articles
  resources :merge
  resources :jenkins

  match '/start',  to: 'builder#new',            via: 'post'
  match '/articles/submit',  to: 'articles#submit',            via: 'post'
  match '/merge/new',  to: 'merge#new',            via: 'post'
  match '/jenkins/new',  to: 'jenkins#new',            via: 'post'

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
  root  'builder#index'
end
