module PE1 where

import Text.Printf
import Data.List

-- Type synonyms --
type Point = (Double, Double)
type Signal = (Double, Double, Double, Double)

-- This function takes a Double and rounds it to 2 decimal places as requested in the PE --
getRounded :: Double -> Double
getRounded x = read s :: Double
               where s = printf "%.2f" x

-------------------------------------------------------------------------------------------
----------------------- DO NOT CHANGE ABOVE OR FUNCTION SIGNATURES-------------------------
------------- DUMMY IMPLEMENTATIONS ARE GIVEN TO PROVIDE A COMPILABLE TEMPLATE ------------
------------------- REPLACE THEM WITH YOUR COMPILABLE IMPLEMENTATIONS ---------------------
-------------------------------------------------------------------------------------------

getDistance :: Point -> Point -> Double
getDistance (x1, y1) (x2, y2) = getRounded $ ((y1-y2)**2 + (x1-x2)**2)**0.5

-------------------------------------------------------------------------------------------

findAllDistances :: Point -> [Point] -> [Double]
findAllDistances _ [] =  []
findAllDistances b (l:ls) = getDistance b l : findAllDistances b ls

-------------------------------------------------------------------------------------------

smallestIndices :: Ord a => [a] -> Int
smallestIndices ls = findIndices (== minimum ls) ls !! 0

largestIndices :: Ord a => [a] -> Int
largestIndices ls = findIndices (== maximum ls) ls !! 0


findExtremesHelper :: Point -> [Point] -> (Int, Int)

findExtremesHelper b l = (smallestIndices $ findAllDistances b l , largestIndices $ findAllDistances b l)


findExtremes :: Point -> [Point] -> (Point, Point)
findExtremes b l = (l !! (fst $ findExtremesHelper b l), l !! ( snd $ findExtremesHelper b l))

-------------------------------------------------------------------------------------------
getnth :: (a,a,a,a) -> Int -> a
getnth (a, _, _, _) 1 = a
getnth (_, b, _, _) 2 = b
getnth (_, _, c, _) 3 = c
getnth (_, _, _, d) 4 = d


getSingleAction :: Signal -> String



getSingleAction signal -- 1:NORTH 2:EAST 3:SOUTH 4:WEST
	| (getnth signal 1 - getnth signal 3) == 0 && (getnth signal 2 - getnth signal 4) == 0 = "Stay"
	| (getnth signal 1 - getnth signal 3) > 0 && (getnth signal 2 - getnth signal 4) == 0 = "North"
	| (getnth signal 1 - getnth signal 3) < 0 && (getnth signal 2 - getnth signal 4) == 0 = "South"
	| (getnth signal 1 - getnth signal 3) == 0 && (getnth signal 2 - getnth signal 4) > 0 = "East"
	| (getnth signal 1 - getnth signal 3) == 0 && (getnth signal 2 - getnth signal 4) < 0 = "West"
	| (getnth signal 1 - getnth signal 3) > 0 && (getnth signal 2 - getnth signal 4) > 0 = "NorthEast"
	| (getnth signal 1 - getnth signal 3) > 0 && (getnth signal 2 - getnth signal 4) < 0 = "NorthWest"
	| (getnth signal 1 - getnth signal 3) < 0 && (getnth signal 2 - getnth signal 4) > 0 = "SouthEast"
	| (getnth signal 1 - getnth signal 3) < 0 && (getnth signal 2 - getnth signal 4) < 0 = "SouthWest"



-------------------------------------------------------------------------------------------

getAllActions :: [Signal] -> [String]
getAllActions [] = []
getAllActions (signal:signals) = getSingleAction signal : getAllActions signals

-------------------------------------------------------------------------------------------

numberOfGivenAction :: Num a => [Signal] -> String -> a
numberOfGivenAction [] _ = 0
numberOfGivenAction (signal:signals) action 
	|  getSingleAction signal == action = 1 + numberOfGivenAction signals action 
	| otherwise = numberOfGivenAction signals action 