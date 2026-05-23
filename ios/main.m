/*
 * SDL2 iOS Launcher
 *
 * This is the only Objective-C file needed.
 * SDL_UIKitMain() handles all UIKit / Metal setup internally,
 * then calls the C++ main() via SDL_main.
 */

#import <UIKit/UIKit.h>
#import <SDL.h>

int main(int argc, char *argv[]) {
    @autoreleasepool {
        return SDL_UIKitMain(argc, argv);
    }
}
