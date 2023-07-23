module PE4 where

import Data.List
import Data.Maybe

data Room = SeedStorage Int
          | Nursery Int Int
          | QueensChambers
          | Tunnel
          | Empty
          deriving Show

data Nestree = Nestree Room [Nestree] deriving Show

---------------------------------------------------------------------------------------------
------------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
--------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
--------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
---------------------------------------------------------------------------------------------

-- Note: undefined is a value that causes an error when evaluated. Replace it with
-- a viable definition! Name your arguments as you like by changing the holes: _

---------------------------------------------------------------------------------------------

-- Q1: Calculate the nutrition value of a given nest.

nestNutritionValueHelper :: Nestree -> Int
nestNutritionValueHelper (Nestree (SeedStorage seeds) []) = seeds*3
nestNutritionValueHelper (Nestree (Nursery eggs larvae) []) = eggs*7 + larvae*10
nestNutritionValueHelper (Nestree (QueensChambers) []) = 0
nestNutritionValueHelper (Nestree (Tunnel) []) = 0
nestNutritionValueHelper (Nestree (Empty) []) = 0
nestNutritionValueHelper (Nestree (SeedStorage seeds) (x:xs)) = seeds*3 + nestNutritionValueHelper (x) +  nestNutritionValueHelper (Nestree (Empty) (xs))
nestNutritionValueHelper (Nestree (Nursery eggs larvae) (x:xs)) = eggs*7 + larvae*10 + nestNutritionValueHelper (x) +  nestNutritionValueHelper (Nestree (Empty) (xs))
nestNutritionValueHelper (Nestree (QueensChambers) (x:xs)) = nestNutritionValueHelper (x) +  nestNutritionValueHelper (Nestree (Empty) (xs))
nestNutritionValueHelper (Nestree (Tunnel) (x:xs)) = nestNutritionValueHelper (x) +  nestNutritionValueHelper (Nestree (Empty) (xs))
nestNutritionValueHelper (Nestree (Empty) (x:xs)) = nestNutritionValueHelper (x) +  nestNutritionValueHelper (Nestree (Empty) (xs))



nestNutritionValue :: Nestree -> Int
nestNutritionValue (Nestree room list) =  nestNutritionValueHelper (Nestree room list)



-- Q2: Calculate the nutrition value of each root-to-leaf path.


pathNutritionValuesHelper :: (Nestree,Int,Bool) -> [Int]
pathNutritionValuesHelper ((Nestree (SeedStorage seeds) []),pathLength,True) = (pathLength+seeds*3):[]
pathNutritionValuesHelper ((Nestree (Nursery eggs larvae) []),pathLength,True) = (pathLength+eggs*7+larvae*10):[]
pathNutritionValuesHelper ((Nestree (QueensChambers) []),pathLength,True) = pathLength:[]
pathNutritionValuesHelper ((Nestree (Tunnel) []),pathLength,True) = pathLength:[]
pathNutritionValuesHelper ((Nestree (Empty) []),pathLength,True) = pathLength:[]
pathNutritionValuesHelper ((Nestree (Empty) []),pathLength,False) = []
pathNutritionValuesHelper ((Nestree (SeedStorage seeds) (x:xs)),pathLength,True) = (pathNutritionValuesHelper (x,pathLength+seeds*3,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength+seeds*3,False))
pathNutritionValuesHelper ((Nestree (Nursery eggs larvae) (x:xs)),pathLength,True) = (pathNutritionValuesHelper (x,pathLength+eggs*7+larvae*10,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength+eggs*7+larvae*10,False))
pathNutritionValuesHelper ((Nestree (QueensChambers) (x:xs)),pathLength,True) = (pathNutritionValuesHelper (x,pathLength,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength,False))
pathNutritionValuesHelper ((Nestree (Tunnel) (x:xs)),pathLength,True) = (pathNutritionValuesHelper (x,pathLength,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength,False))
pathNutritionValuesHelper ((Nestree (Empty) (x:xs)),pathLength,False) = (pathNutritionValuesHelper (x,pathLength,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength,False))
pathNutritionValuesHelper ((Nestree (Empty) (x:xs)),pathLength,True) = (pathNutritionValuesHelper (x,pathLength,True))++(pathNutritionValuesHelper (((Nestree (Empty) (xs))),pathLength,False))



pathNutritionValues (Nestree room list) =  pathNutritionValuesHelper ((Nestree room list),0,True)

-- Q3: Find the depth of the shallowest tunnel, if you can find one.


shallowestTunnelHelper :: (Nestree,Int) -> [Int]
shallowestTunnelHelper ((Nestree (SeedStorage seeds) []),depth) = []
shallowestTunnelHelper ((Nestree (Nursery eggs larvae) []),depth) = []
shallowestTunnelHelper ((Nestree (QueensChambers) []),depth) = []
shallowestTunnelHelper ((Nestree (Tunnel) []),depth) = depth:[]
shallowestTunnelHelper ((Nestree (Empty) []),depth) = []
shallowestTunnelHelper ((Nestree (SeedStorage seeds) (x:xs)),depth) = (shallowestTunnelHelper (x,depth+1))++(shallowestTunnelHelper (((Nestree (Empty) (xs))),depth))
shallowestTunnelHelper ((Nestree (Nursery eggs larvae) (x:xs)),depth) = (shallowestTunnelHelper (x,depth+1))++(shallowestTunnelHelper (((Nestree (Empty) (xs))),depth))
shallowestTunnelHelper ((Nestree (QueensChambers) (x:xs)),depth) = (shallowestTunnelHelper (x,depth+1))++(shallowestTunnelHelper (((Nestree (Empty) (xs))),depth))
shallowestTunnelHelper ((Nestree (Tunnel) (x:xs)),depth) = depth:(shallowestTunnelHelper (x,depth+1))++(shallowestTunnelHelper (((Nestree (Empty) (xs))),depth))
shallowestTunnelHelper ((Nestree (Empty) (x:xs)),depth) = (shallowestTunnelHelper (x,depth+1))++(shallowestTunnelHelper (((Nestree (Empty) (xs))),depth))




shallowestTunnel :: Nestree -> Maybe Int
shallowestTunnel (Nestree room list)
			| (shallowestTunnelHelper ((Nestree room list),1)) == [] = Nothing
			| otherwise = Just( minimum (shallowestTunnelHelper ((Nestree room list),1)))
			



-- Q4: Find the path to the Queen's Chambers, if such a room exists.


pathToQueenHelper :: (Nestree,[Room]) -> [Room]
pathToQueenHelper ((Nestree (SeedStorage seeds) []),path) = []
pathToQueenHelper ((Nestree (Nursery eggs larvae) []),path) = []
pathToQueenHelper ((Nestree (QueensChambers) []),path) = path++[(QueensChambers)]
pathToQueenHelper ((Nestree (Tunnel) []),path) = []
pathToQueenHelper ((Nestree (Empty) []),path) = []
pathToQueenHelper ((Nestree (SeedStorage seeds) (x:xs)),path) = (pathToQueenHelper (x,path++[(SeedStorage seeds)]))++(pathToQueenHelper (((Nestree (SeedStorage seeds) (xs))),path))
pathToQueenHelper ((Nestree (Nursery eggs larvae) (x:xs)),path) = (pathToQueenHelper (x,path++[(Nursery eggs larvae)]))++(pathToQueenHelper (((Nestree (Nursery eggs larvae) (xs))),path))
pathToQueenHelper ((Nestree (QueensChambers) (x:xs)),path) = path++[(QueensChambers)]
pathToQueenHelper ((Nestree (Tunnel) (x:xs)),path) = (pathToQueenHelper (x,path++[(Tunnel)]))++(pathToQueenHelper (((Nestree (Tunnel) (xs))),path))
pathToQueenHelper ((Nestree (Empty) (x:xs)),path) = (pathToQueenHelper (x,path++[(Empty)]))++(pathToQueenHelper (((Nestree (Empty) (xs))),path))

pathToQueen2 :: Nestree -> [Room]
pathToQueen2 (Nestree room list) = pathToQueenHelper ((Nestree room list),[])

pathToQueen :: Nestree -> Maybe [Room]
pathToQueen (Nestree room list)
	| length(pathToQueen2 (Nestree room list)) ==  0 = Nothing
	| length(pathToQueen2 (Nestree room list)) ==  1 = Just []
	| otherwise = Just(init(pathToQueen2 (Nestree room list)))


-- Q5: Find the quickest depth to the Queen's Chambers, including tunnel-portation :)
--quickQueenDepth :: Nestree -> Maybe Int
--quickQueenDepth _ = undefined 


findLastTunnel ::(Room,[Room],Int, Int) -> Int
findLastTunnel ((Tunnel),[],index,i) = i
findLastTunnel ((SeedStorage seeds),[],index,i) = index
findLastTunnel ((Nursery eggs larvae),[],index,i) = index
findLastTunnel ((QueensChambers),[],index,i) = index
findLastTunnel ((Empty),[],index,i) = index
findLastTunnel ((Tunnel),(x:xs),index,i) = findLastTunnel (x, xs, i, i+1)
findLastTunnel ((SeedStorage seeds),(x:xs),index,i) = findLastTunnel (x, xs, index, i+1)
findLastTunnel ((Nursery eggs larvae),(x:xs),index,i) = findLastTunnel (x, xs, index, i+1)
findLastTunnel ((QueensChambers),(x:xs),index,i) = findLastTunnel (x, xs, index, i+1)
findLastTunnel ((Empty),(x:xs),index,i) = findLastTunnel (x, xs, index, i+1)

quickQueenDepthHelper :: (Nestree,[Room]) -> [Room]
quickQueenDepthHelper ((Nestree (SeedStorage seeds) []),path) = []
quickQueenDepthHelper ((Nestree (Nursery eggs larvae) []),path) = []
quickQueenDepthHelper ((Nestree (QueensChambers) []),path) = path++[(QueensChambers)]
quickQueenDepthHelper ((Nestree (Tunnel) []),path) = []
quickQueenDepthHelper ((Nestree (Empty) []),path) = []
quickQueenDepthHelper ((Nestree (SeedStorage seeds) (x:xs)),path) = (quickQueenDepthHelper (x,path++[(SeedStorage seeds)]))++(quickQueenDepthHelper (((Nestree (SeedStorage seeds) (xs))),path))
quickQueenDepthHelper ((Nestree (Nursery eggs larvae) (x:xs)),path) = (quickQueenDepthHelper (x,path++[(Nursery eggs larvae)]))++(quickQueenDepthHelper (((Nestree (Nursery eggs larvae) (xs))),path))
quickQueenDepthHelper ((Nestree (QueensChambers) (x:xs)),path) = path++[(QueensChambers)]
quickQueenDepthHelper ((Nestree (Tunnel) (x:xs)),path) = (quickQueenDepthHelper (x,path++[(Tunnel)]))++(quickQueenDepthHelper (((Nestree (Tunnel) (xs))),path))
quickQueenDepthHelper ((Nestree (Empty) (x:xs)),path) = (quickQueenDepthHelper (x,path++[(Empty)]))++(quickQueenDepthHelper (((Nestree (Empty) (xs))),path))

quickQueenDepth2 :: Nestree -> [Room]
quickQueenDepth2 (Nestree room list) = quickQueenDepthHelper ((Nestree room list),[])

quickQueenDepth :: Nestree -> Maybe Int
quickQueenDepth (Nestree room list)
	| length(quickQueenDepth2 (Nestree room list)) ==  0 = Nothing
	| ((findLastTunnel ((head (quickQueenDepth2 (Nestree room list))), (tail (quickQueenDepth2 (Nestree room list))), -1, 1)) == -1) = Just(length(quickQueenDepth2 (Nestree room list)))
	| otherwise = Just(length(quickQueenDepth2 (Nestree room list))- ((findLastTunnel ((head (quickQueenDepth2 (Nestree room list))), (tail (quickQueenDepth2 (Nestree room list))), 1, 1)) - (fromMaybe 0 (shallowestTunnel(Nestree room list)))))



-- Example nest given in the PDF.
exampleNest :: Nestree
exampleNest = 
  Nestree Tunnel [
    Nestree (SeedStorage 15) [
      Nestree (SeedStorage 81) []
    ],
    Nestree (Nursery 8 16) [
      Nestree Tunnel [
        Nestree QueensChambers [
          Nestree (Nursery 25 2) []
        ]
      ]
    ],
    Nestree Tunnel [
      Nestree Empty [],
      Nestree (SeedStorage 6) [
        Nestree Empty [],
        Nestree Empty []
      ]
    ]
  ]

-- Same example tree, with tunnels replaced by Empty
exampleNestNoTunnel :: Nestree
exampleNestNoTunnel = 
  Nestree Empty [
    Nestree (SeedStorage 15) [
      Nestree (SeedStorage 81) []
    ],
    Nestree (Nursery 8 16) [
      Nestree Empty [
        Nestree QueensChambers [
          Nestree (Nursery 25 2) []
        ]
      ]
    ],
    Nestree Empty [
      Nestree Empty [],
      Nestree (SeedStorage 6) [
        Nestree Empty [],
        Nestree Empty []
      ]
    ]
  ]
