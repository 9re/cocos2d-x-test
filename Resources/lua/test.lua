-- overide lua print with CCLuaLog
print = CCLuaLog

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
    CCDirector:sharedDirector():endToLua()
end



local function main()
     -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
    
    local cclog = function(...)
        print(string.format(...))
    end
  
    local function menuCallbackClosePopup()
        cclog("menu clicked!");
        CCDirector:sharedDirector():endToLua()
    end
  
    local visibleSize = CCDirector:sharedDirector():getVisibleSize()
    local origin = CCDirector:sharedDirector():getVisibleOrigin()
    local winSize = CCDirector:sharedDirector():getWinSize()
  
    local menuPopupItem = CCMenuItemImage:create("images/CloseNormal.png", "images/CloseSelected.png")
    menuPopupItem:registerScriptTapHandler(menuCallbackClosePopup)
    
    local menu = CCMenu:createWithItem(menuPopupItem)
    local sceneGame = CCScene:create()
    local stageLayer = BackKeyLayer:create()
    stageLayer:addChild(menu)
    sceneGame:addChild(stageLayer)
  
    local numTargets = 0
    local targetsTable = {}
  
    local function removeTarget(sprite)
        stageLayer:removeChild(sprite, true)
        targetsTable[sprite.id] = nil
    end
    
    local function spriteMoveFinished(sprite)
        removeTarget(sprite)
    end
  
    local function add_target()
        local target = CCSprite:create("images/konchi.png")
        local xPadding = 80
        local bandWith = (winSize.width - xPadding) / 4
        local xPos = math.random(4) * bandWith + xPadding / 2 - target:getContentSize().width / 2
                
        target:setPosition(xPos, winSize.height + target:getContentSize().height / 2)
        stageLayer:addChild(target)
                
        local minDuration = 2
        local maxDuration = 4
        local rangeDuration = maxDuration - minDuration
        local actualDuration = math.random(rangeDuration) + minDuration
                
        local actionMove = CCMoveTo:create(actualDuration, ccp(xPos, -target:getContentSize().height / 2))
        local actionMoveDone = CCCallFuncN:create(spriteMoveFinished)
        local actions = CCArray:createWithCapacity(2)
                
        actions:addObject(actionMove)
        actions:addObject(actionMoveDone)
        target:runAction(CCSequence:create(actions))
                
        local id = "obj"..numTargets
        numTargets = numTargets + 1
        target.id = id
        targetsTable[id] = target
        cclog("added to table: %d", numTargets)
    end
  
    local function gameLogic(t)
        add_target()
    end
  
    local function hitTest(px, py, x, y)
        --[[
        if math.abs(py - winSize.height / 2) > 40 then
          return false
        end
        --]]
        
        local dx = px - x
        local dy = py - y
        if dx * dx + dy * dy < 1600 then
          return true
        end
    end
  
    local function hitEffect(t)
        local actionEffect = CCTintTo:create(0.4, 255, 255, 255)
        local actionMoveDone = CCCallFuncN:create(spriteMoveFinished)
        local actions = CCArray:createWithCapacity(2)
                
        local explosion = CCParticleSmoke:create()
        explosion:setTexture(CCTextureCache:sharedTextureCache():addImage("images/fire.png"))
        local explosionNode = tolua.cast(explosion, "CCNode")
        explosionNode:setPosition(t:getPosition())
        stageLayer:addChild(explosionNode)
                
        actions:addObject(actionEffect)
        actions:addObject(actionMoveDone)
        t:runAction(CCSequence:create(actions))
    end
  
    local function onTouchBegan(x, y)
        cclog("onTouchBegan: %0.2f, %0.2f", x, y)
                
        for id, t in pairs(targetsTable) do
            local px, py = t:getPosition()
            
            if hitTest(px, py, x, y) then
                t:stopAllActions()
                --removeTarget(t)
                hitEffect(t)
            end
        end
        
        return true
    end
    
    local function onTouchMoved(x, y)
        --cclog("onTouchMoved: %0.2f, %0.2f", x, y)
    end
  
    local function onTouchEnded(x, y)
        --cclog("onTouchEnded: %0.2f, %0.2f", x, y)
    end
    
    local function onTouch(eventType, x, y)
        if eventType == CCTOUCHBEGAN then
            return onTouchBegan(x, y)
        elseif eventType == CCTOUCHMOVED then
            return onTouchMoved(x, y)
        else
            return onTouchEnded(x, y)
        end
    end
  
    stageLayer:registerScriptTouchHandler(onTouch)
    stageLayer:setTouchEnabled(true)
  
    CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(gameLogic, 1.0, false)
    CCDirector:sharedDirector():runWithScene(sceneGame)
end

xpcall(main, __G__TRACKBACK__)
