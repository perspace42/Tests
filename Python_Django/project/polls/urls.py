from django.urls import path
from . import views

#differentiate these routes from apps with routes that have the same names
app_name = "polls"

#record routes
urlpatterns = [
    # ex: /polls/
    path("", views.IndexView.as_view(), name="index"),
    # ex: /polls/5/
    path("<int:pk>/", views.DetailView.as_view(), name="detail"), #pk is primary key captured from URL
    # ex: /polls/5/results/
    path("<int:pk>/results/", views.ResultsView.as_view(), name="results"), #pk is primary key captured from URL
    # ex: /polls/5/vote/
    path("<int:question_id>/vote/", views.vote, name="vote"),
]
