module PE2 where

import Text.Printf

type Point = (Int, Int)
type Dimensions = (Int, Int)
type Vector = (Int, Int)

getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

castIntToDouble x = (fromIntegral x) :: Double

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------------------------------------------
getVector :: String -> Vector
getVector d	
	| d == "Stay" = (0,0)
	| d == "North" = (0,1)
	| d == "South" = (0,-1)
	| d == "East" = (1,0)
	| d == "West" = (-1,0)
	| d == "NorthEast" = (1,1)
	| d == "NorthWest" = (-1,1)
	| d == "SouthEast" = (1,-1)
	| d == "SouthWest" = (-1,-1)

-------------------------------------------------------------------------------------------------------------------------------



getAllVectors :: [String] -> [Vector]
getAllVectors [] = []
getAllVectors (l:ls) = getVector l : getAllVectors ls

-------------------------------------------------------------------------------------------------------------------------------


producePathHelper :: Point -> [String] -> [Point]
producePathHelper _ [] = []
producePathHelper initial (l:ls) = (fst initial + fst (getVector l), snd initial + snd (getVector l)) : 
									producePathHelper (fst initial + fst (getVector l), snd initial + snd (getVector l)) ls
							  
producePath :: Point -> [String] -> [Point]
producePath initial actions = initial : producePathHelper initial actions

-------------------------------------------------------------------------------------------------------------------------------
takePathInArea :: [Point] -> Dimensions -> [Point]
takePathInArea [] _ = []
takePathInArea (path:paths) (bx, by)
	| (fst path < bx && snd path < by && fst path >= 0 && snd path >= 0) = path : takePathInArea paths (bx,by)
	| otherwise = []

	

-------------------------------------------------------------------------------------------------------------------------------
setDifference :: (Eq a) => [a] -> [a] -> [a]
setDifference xs excludes = filter (not . (`elem` excludes)) xs

remainingObjectsHelper :: [Point] -> Dimensions -> [Point] -> [Point]
remainingObjectsHelper path border objects = setDifference objects path


remainingObjects :: [Point] -> Dimensions -> [Point] -> [Point]
remainingObjects path border objects = remainingObjectsHelper (takePathInArea path border) border objects

-------------------------------------------------------------------------------------------------------------------------------
	
isInsideTheDimesion :: [Point] -> Dimensions -> Bool
isInsideTheDimesion [] _ = True
isInsideTheDimesion (path:paths) (bx, by)
    | (fst path < bx && snd path < by && fst path >= 0 && snd path >= 0) = isInsideTheDimesion paths (bx,by)
    | otherwise = False



averageStepsInSuccessHelper :: [[Point]] -> Dimensions -> [Point] -> Int
averageStepsInSuccessHelper [] _ _ = 0
averageStepsInSuccessHelper (path:paths) border objects
    | (isInsideTheDimesion path border) && (remainingObjects path border objects == []) = (length path) + (averageStepsInSuccessHelper paths border objects)
    | otherwise = averageStepsInSuccessHelper paths border objects
	
totalSuccess :: [[Point]] -> Dimensions -> [Point] -> Int
totalSuccess [] _ _ = 0
totalSuccess (path:paths) border objects
    | (isInsideTheDimesion path border) && (remainingObjects path border objects == []) = 1 + (totalSuccess paths border objects)
    | otherwise = totalSuccess paths border objects
	
averageStepsInSuccess :: [[Point]] -> Dimensions -> [Point] -> Double
averageStepsInSuccess paths border objects = (castIntToDouble(averageStepsInSuccessHelper paths border objects)) / (castIntToDouble(totalSuccess paths border objects))