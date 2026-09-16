-- config.lua
-- This acts as easily modifiable configuration for our C program

window_title = "Lua Integration Demo"
window_width = 1920
window_height = 1080

-- We can call the custom C function we registered earlier!
print_warning("Lua script loaded successfully. Beginning logic evaluation.")

-- The C program will call this function every frame to determine timing
function calculate_frame_delay(target_fps, active_entities)
    local base_delay = 1000.0 / target_fps
    
    -- Arbitrary logic: slow down if there are too many entities on screen
    if active_entities > 1000 then
        print_warning("High entity count detected! Throttling frame rate.")
        return base_delay * 1.5 
    end
    
    return base_delay
end