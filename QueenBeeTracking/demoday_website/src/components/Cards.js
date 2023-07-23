import React from "react";
import "./Cards.css";
import CardItem from "./CardItem";

function Cards() {
	return (
		<div className="cards">
			<div align="center">
				<h1>Watch our demo !</h1>
				<iframe
					width="600"
					height="400"
					src="https://www.youtube.com/embed/-H6tY7r0vkw"
					title="Queenbee Tracker Video"
					frameborder="0"
					allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
					allowfullscreen
				></iframe>
			</div>

			<h1 className="solution-header">Our Solution</h1>
			<div className="cards__container">
				<div className="cards__wrapper">
					<ul className="cards__items"></ul>
					<ul className="cards__items">
						<CardItem
							src="images/preview_000013.png"
							text="We are extracting frames from the rosbag files so that scientist can view them using our web application "
							label="Rosbags"
							path="/"
						/>
						<CardItem
							src="images/detection.png"
							text="Extracted images are given as a input to our object detection model which outputs the location and angle of the bees in the hive. Biologists can view detections on frames using our web application."
							label="Detection"
							path="/"
						/>
					</ul>
					<ul className="cards__items">
						<CardItem
							src="images/000012.gif"
							text="Researchers can use our tracking model for tracking movement of the queen bee."
							label="Tracking"
							path="/"
						/>
						<CardItem
							src="images/analysis.png"
							text="We are processing extracted frames and detections for creating analysis about queen bee and court bee movements and behavior. We display the analyses in the form of several charts and graphs."
							label="Analyses"
							path="/"
						/>
					</ul>
				</div>
			</div>
		</div>
	);
}

export default Cards;
