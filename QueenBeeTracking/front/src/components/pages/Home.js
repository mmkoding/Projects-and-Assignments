import * as React from "react";
import Button from "@mui/material/Button";
import axios from 'axios';
import CssBaseline from "@mui/material/CssBaseline";
import TextField from "@mui/material/TextField";
import Box from "@mui/material/Box";
import Typography from "@mui/material/Typography";
import Container from "@mui/material/Container";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import { useHistory } from 'react-router-dom'; 

// TODO remove, this demo shouldn't need to reset the theme.


const defaultTheme = createTheme();

export default function SignIn() {
	const history = useHistory();

	const handleSubmit = async (event) => {
		event.preventDefault();
		const data = new FormData(event.currentTarget);
		const username = data.get('username');
		const password = data.get('password');
	
		try {
			const response = await axios.post(
			  `http://144.122.48.179:8000/login`,
			  {
				username: username,
				password: password
			  }
			);
			if(response.status === 200){
				window.localStorage.setItem('isAuthenticated', 'true');
				history.push('/manage');
				console.log(response);
				return
			}
			alert('Login failed, please try again')
		} catch (error) {
			console.error("Failed to run detection", error);
			alert('Login failed, please try again')
		}

	};

	return (
		<ThemeProvider theme={defaultTheme}>
			<Container component="main" maxWidth="xs">
				<CssBaseline />
				<Box
					sx={{
						marginTop: 8,
						display: "flex",
						flexDirection: "column",
						alignItems: "center",
					}}
				>
					<img src={`${process.env.PUBLIC_URL}/favicon.ico`} alt="QueenBee Icon" style={{margin: '1rem', borderRadius: '50%'}}/>
					<Typography component="h1" variant="h5">
						Sign in
					</Typography>
					<Box
						component="form"
						onSubmit={handleSubmit}
						noValidate
						sx={{ mt: 1 }}
					>
						<TextField
							margin="normal"
							required
							fullWidth
							id="username"
							label="Username"
							name="username"
							autoComplete="username"
							autoFocus
						/>
						<TextField
							margin="normal"
							required
							fullWidth
							name="password"
							label="Password"
							type="password"
							id="password"
							autoComplete="current-password"
						/>
							<Button
								type="submit"
								fullWidth
								variant="contained"
								sx={{ mt: 3, mb: 2 }}
							>
								Sign In
							</Button>
					</Box>
				</Box>
			</Container>
		</ThemeProvider>
	);
}
