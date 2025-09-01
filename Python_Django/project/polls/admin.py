from django.contrib import admin #for adding admin access to objects
from .models import Question, Choice #import the object to be added

#Older way to edit Questions and Choices
#admin.site.register(Question)
#admin.site.register(Choice)

#Use admin.StackedInline to display vertically instead
class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3 #Adds a number of extra blank choices equal to number

class QuestionAdmin(admin.ModelAdmin):
    #Fields are passed in the order provided in the list
    fieldsets = [
        #Title, 
        (None, {"fields": ["question_text"]}),
        ("Date information", {"fields": ["pub_date"]}),
    ]
    #Modify the how the list of questions displays its data
    list_display = ["question_text", "pub_date", "was_published_recently"]

    #Have Choices Contained in Questions
    inlines = [ChoiceInline]

    #Add Filter To List
    list_filter = ["pub_date"]

    #Add Search Capability
    search_fields = ["question_text"]


#Register
admin.site.register(Question, QuestionAdmin)