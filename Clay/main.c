#include "include/raylib.h"
#define TITLE "CLAY EXAMPLE PROGRAM"
#define FONT "fonts/Roboto-Regular.ttf"
#define FONT_ID_BODY_16 0
#define FONT_ID_BODY_24 1
#define HEIGHT 800
#define WIDTH 600
//CLAY_IMPLEMENTATION must go above any clay imports 
#define CLAY_IMPLEMENTATION

#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.c"


//Structures

typedef struct {
    Clay_String title;
    Clay_String contents;
}Document;

typedef struct {
    Document *documents;
    uint32_t length;
}DocumentArray;

//Variables

//Data
Document documentsArray[] = {
    {.title = CLAY_STRING("Avatar The Last Airbender"), .contents = CLAY_STRING("Avatar: The Last Airbender, also known as Avatar: The Legend of Aang in some regions, is an American animated fantasy action television series created by Michael Dante DiMartino and Bryan Konietzko and produced by Nickelodeon Animation Studio. Avatar is set in a largely Asian-inspired world in which some people can telekinetically manipulate one of the four elements—water, earth, fire or air—through practices known as 'bending', inspired by Chinese martial arts. The only individual who can bend all four elements, the 'Avatar', is responsible for maintaining harmony among the world's four nations, and serves as the link between the physical world and the spirit world. The series follows the journey of twelve-year-old Aang, the current Avatar and last survivor of his nation, the Air Nomads, along with his friends Katara, Sokka, and Toph, as they strive to end the Fire Nation's war against the other nations and defeat Fire Lord Ozai before he conquers the world. The series also follows Zuko—the exiled prince of the Fire Nation, seeking to restore his lost honor by capturing Aang, accompanied by his uncle Iroh—and later, his sister Azula. Avatar is presented in a style that combines anime with American cartoons and relies on the imagery of primarily Chinese culture,[2][3] with various other influences from different East Asian, Southeast Asian, South Asian, North Asian, and Native American cultures. Avatar: The Last Airbender was a ratings success and received acclaim from critics for its characters, cultural references, art direction, voice acting, soundtrack, humor, and themes. The series' themes include concepts rarely touched on in youth entertainment, including war, genocide, imperialism, totalitarianism, indoctrination and free choice.[4] It won five Annie Awards, a Genesis Award, a Primetime Emmy Award, a Kids' Choice Award, and a Peabody Award. The show is regarded by many critics as one of the greatest animated television series of all time. Avatar aired on Nickelodeon for three seasons, from February 2005 to July 2008.[5] The extended Avatar franchise includes an ongoing comics series, a prequel novel series, an animated sequel series, and a live-action film, as well as a live-action remake series produced for Netflix.[6] The complete series was released on Blu-ray in June 2018 in honor of the tenth anniversary of its finale[7] and was made available to stream on Netflix in the United States and Canada in May 2020,[8][9] on Paramount+ in June 2020,[10] and on Amazon Prime Video in January 2021.[11]")},
    {.title = CLAY_STRING("Rome"), .contents = CLAY_STRING("HBO Epic Mini Series On The Fall of The Roman Republic")},
    {.title = CLAY_STRING("Star Wars The Clone Wars"), .contents = CLAY_STRING("2010's CGI Animated Masterpiece of Star Wars")},
    {.title = CLAY_STRING("Teen Titans"), .contents = CLAY_STRING("When there's trouble you know who to call")},
    {.title = CLAY_STRING("Justice League Unlimited"), .contents = CLAY_STRING("The real DC expanded universe")}
};

//Data Storage
DocumentArray documents = {
    .documents = documentsArray,
    .length = 5
};

//Track Selected Document
uint32_t selectedDocIndex = 0;

//Track Visibility of File Menu
bool fileMenuVisible;

//Functions

void handleError(Clay_ErrorData error){
    fprintf(stderr,"Error Code: %d \nError %s",error.errorType, error.errorText.chars);
    exit(1);
}

void RenderHeaderButton(Clay_String text){
    CLAY(
        CLAY_LAYOUT({
            .padding = {.left = 16, .right = 16, .top = 8, .bottom = 8}
        }),
        CLAY_RECTANGLE({
            .color = {.r = 140, .g = 140, .b = 140, .a = 255},
            .cornerRadius = 5
        })
    ){
        CLAY_TEXT(text,CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = {.r = 255, .g = 255, .b = 255, .a = 255}
        }));
    }
}

void RenderDropDownMenuItem(Clay_String text){
    CLAY(
        CLAY_LAYOUT({
            .padding = {.left = 16, .right = 16, .top = 8, .bottom = 8}
        })
    ){
        CLAY_TEXT(text,CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = {.r = 255, .g = 255, .b = 255, .a = 255}
        }));
    }
}

//Handle Mouse Interaction With Sidebar Buttons
void HandleSidebarInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData){
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        if(userData >= 0 && userData < documents.length){
            selectedDocIndex = userData;
        }
    }
}

int main(){
    //Load Debug Tools
    //Clay_SetDebugModeEnabled(1);
    //Initialize Renderer
    Clay_Raylib_Initialize(WIDTH,HEIGHT,TITLE,FLAG_WINDOW_RESIZABLE);
    //Setup Memory
    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena){
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };
    //Setup Clay
    Clay_Initialize(clayMemory, 
        (Clay_Dimensions){ 
            .width = GetScreenWidth(), 
            .height = GetScreenHeight() 
        }, 
        (Clay_ErrorHandler){ 
            handleError, 0 
        }
    );
    //Load Text
    Clay_SetMeasureTextFunction(Raylib_MeasureText);
    //Setup Fonts (Remember a Separate Font For Each Different Font Size)
    Raylib_fonts[FONT_ID_BODY_16] = (Raylib_Font) {
        .font = LoadFontEx(FONT,16,0,400),
        .fontId = FONT_ID_BODY_16
    };
    Raylib_fonts[FONT_ID_BODY_24] = (Raylib_Font) {
        .font = LoadFontEx(FONT,24,0,400),
        .fontId = FONT_ID_BODY_24
    };

    while (WindowShouldClose() == false){
        //Run Once per Frame
        Clay_SetLayoutDimensions((Clay_Dimensions){
            .width = GetScreenWidth(),
            .height = GetScreenHeight()
        });

        //Get Mouse Position (From Raylib)
        Vector2 mousePosition = GetMousePosition();
        Vector2 scrollDelta = GetMouseWheelMoveV();
        //Inform Clay Where Mouse Position Is
        Clay_SetPointerState(
            (Clay_Vector2) {mousePosition.x, mousePosition.y},
            IsMouseButtonDown(0)
        );
        Clay_UpdateScrollContainers(
            true, //enable touch scrolling
            (Clay_Vector2) {scrollDelta.x,scrollDelta.y}, //scroll wheel location
            GetFrameTime() //time passed since last frame
        );

        //Store Layout
        Clay_Sizing layoutExpand = {
            .width = CLAY_SIZING_GROW(0),
            .height = CLAY_SIZING_GROW(0)
        };

        //Store Background
        Clay_RectangleElementConfig contentBackgroundConfig = {
            .color = { .r = 90, .g = 90, .b = 90, .a = 255},
            .cornerRadius = 8
        };

        //Store SideBar Button
        Clay_LayoutConfig sidebarButtonLayout = {
            .sizing = {.width = CLAY_SIZING_GROW(0)},
            .padding = {.left = 16, .right = 16, .top = 16, .bottom = 16}
        };

        Clay_BeginLayout();
        // Build UI Here
        CLAY(
            CLAY_ID("OuterContainer"),
            CLAY_RECTANGLE({ 
                .color = {.r = 43, .g = 41, .b = 51, .a = 255}
            }), 
            CLAY_LAYOUT({
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = layoutExpand,
                //padding affects Child Elements
                .padding = {.left = 16, .right = 16, .top = 16, .bottom = 16},
                .childGap = 16 
            })
        //UI Elements (Children) Go Inside Of Braces Of Original Clay Statement
        ){
            //Child Elements Go Inside Braces Of Subsequent Clay Statements
            CLAY(
                CLAY_ID("HeaderBar"),
                CLAY_RECTANGLE(contentBackgroundConfig),
                CLAY_LAYOUT({
                    .sizing = {
                        .height = CLAY_SIZING_FIXED(60),
                        .width = CLAY_SIZING_GROW(0)
                    },
                    .padding = {.left = 16, .right = 16, .top = 0, .bottom = 0},
                    .childGap = 16,
                    .childAlignment = {
                        .y = CLAY_ALIGN_Y_CENTER
                    }
                })
            ){
                CLAY(
                    CLAY_ID("FileButton"),
                    CLAY_LAYOUT({
                        .padding = {.left = 16, .right = 16, .top = 8, .bottom = 8}
                    }),
                    CLAY_RECTANGLE({
                        .color = {.r = 140, .g = 140, .b = 140, .a = 255},
                        .cornerRadius = 5
                    })
                ){
                    CLAY_TEXT(CLAY_STRING("File"),CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 16,
                        .textColor = {.r = 255, .g = 255, .b = 255, .a = 255}
                    }));

                    fileMenuVisible = 
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileButton")))
                    ||
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileMenu")));

                    if (fileMenuVisible){
                        //DropDown
                        CLAY(
                            CLAY_ID("FileMenu"),
                            //Floating Elements Do Not Cause Their Parents to Expand or Affect Sibling Elements
                            //By Default they also attach to the top left corner of their parents
                            CLAY_FLOATING({
                                .attachment = {
                                    .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM
                                }
                            }), 
                            CLAY_RECTANGLE({.color = {.r = 40, .g = 40, .b = 40, .a = 255}}),
                            CLAY_LAYOUT({
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .sizing = {
                                    .width = CLAY_SIZING_FIXED(200)
                                }
                            })
                        ){
                            //Render Dropdown Items Here
                            RenderDropDownMenuItem(CLAY_STRING("New"));
                            RenderDropDownMenuItem(CLAY_STRING("Open"));
                            RenderDropDownMenuItem(CLAY_STRING("Close"));
                        }
                    }
                }
                
                RenderHeaderButton(CLAY_STRING("File"));
                RenderHeaderButton(CLAY_STRING("Edit"));
                //Padding To Right Align Remaining Buttons
                CLAY(CLAY_LAYOUT({.sizing = {.width = CLAY_SIZING_GROW(0)}})) {}
                RenderHeaderButton(CLAY_STRING("Upload"));
                RenderHeaderButton(CLAY_STRING("Media"));
                RenderHeaderButton(CLAY_STRING("Support"));
            }
            CLAY(
                CLAY_ID("LowerContent"),
                CLAY_LAYOUT({
                    .sizing = layoutExpand,
                    .childGap = 16 
                })
            ){
                CLAY(
                    CLAY_ID("Sidebar"),
                    CLAY_RECTANGLE(contentBackgroundConfig),
                    CLAY_LAYOUT({
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .padding = {.left = 16, .right = 16, .top = 16, .bottom = 16},
                        .childGap = 8,
                        .sizing = {
                            .width = CLAY_SIZING_FIXED(250),
                            .height = CLAY_SIZING_GROW(0)
                        }
                    })
                ){
                    //Render Document Titles 
                    for (int i = 0; i < documents.length; i++){
                        Document doc = documents.documents[i];
                        if (i == selectedDocIndex){
                            CLAY(
                                CLAY_LAYOUT(sidebarButtonLayout),
                                CLAY_RECTANGLE({
                                    .color = {.r = 120, .g = 120, .b = 120, .a = 255},
                                    .cornerRadius = 8
                                })
                            ){
                                CLAY_TEXT(doc.title, CLAY_TEXT_CONFIG({
                                    .fontId = FONT_ID_BODY_16,
                                    .fontSize = 16,
                                    .textColor = {.r = 255, .g = 255, .b = 255, .a = 255} 
                                }));
                            } 
                        }else{
                            CLAY(
                                CLAY_LAYOUT(sidebarButtonLayout),
                                Clay_OnHover(HandleSidebarInteraction,i),
                                //If Option is Being Hovered Over
                                Clay_Hovered() 
                                    ? CLAY_RECTANGLE({
                                        .color = {.r = 120, .g = 120, .b = 120, .a = 120},
                                        .cornerRadius = 8
                                    }) 
                                    : 0
                            ){
                                CLAY_TEXT(doc.title, CLAY_TEXT_CONFIG({
                                    .fontId = FONT_ID_BODY_16,
                                    .fontSize = 16,
                                    .textColor = {.r = 255, .g = 255, .b = 255, .a = 255} 
                                }));
                            }
                        } 
                    }
                }
                //BUG: Currently Text Does Not Correctly Wrap On Right Hand Side of Window
                CLAY(
                    CLAY_ID("MainContent"),
                    CLAY_RECTANGLE(contentBackgroundConfig),
                    CLAY_SCROLL({.vertical = true}), //Hides any content outside the boundaries of the box
                    CLAY_LAYOUT({ 
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childGap = 16,
                        .padding = {.left = 16, .right = 16, .top = 16, .bottom = 16},
                        .sizing = layoutExpand, //was layoutExpand
                    })
                ){
                    Document selectedDoc = documents.documents[selectedDocIndex];
                    //Render Document Title
                    CLAY_TEXT(selectedDoc.title, CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BODY_24,
                        .fontSize = 24,
                        .textColor = {.r = 255, .g = 255, .b = 255, .a = 255}
                    }));
                    //Render Document Text
                    CLAY_TEXT(selectedDoc.contents, CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BODY_24,
                        .fontSize = 24,
                        .textColor = {.r = 255, .g = 255, .b = 255, .a = 255}
                    }));
                }
            }
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
    }
}