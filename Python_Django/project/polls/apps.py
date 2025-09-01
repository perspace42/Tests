from django.apps import AppConfig

#this is added to INSTALLED_APPS in settings.py to add a reference to the configuration class
class PollsConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'polls'
