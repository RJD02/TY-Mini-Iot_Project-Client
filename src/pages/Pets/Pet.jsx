import React, { useEffect, useState } from "react";
import moment from "moment";
import {
  Alert,
  Button,
  Card,
  CardActions,
  CardContent,
  Snackbar,
  Typography,
} from "@mui/material";
import axios from "axios";

const Pet = (props) => {
  const [feedClicked, setFeedClicked] = useState(false);
  const [message, setMessage] = useState("");
  const { petID, petType, latestRequest, lastFed } = props;

  console.log(moment(lastFed).fromNow());

  useEffect(() => {
    setTimeout(() => {
      setMessage("");
    }, 5000);
  }, [message]);

  const onFeedHandler = async (e) => {
    setFeedClicked(true);
    const data = await axios.post(
      "https://pet-feeder-iot.herokuapp.com/pet/feedNow",
      {
        petID,
      },
      {
        headers: {
          "Content-Type": "application/json",
        },
      }
    );
    console.log(data.data);
    setMessage(data.data.message);
  };

  return (
    <>
      <Card sx={{ minWidth: 275 }}>
        <CardContent>
          <Typography sx={{ fontSize: 14 }} color="text.secondary" gutterBottom>
            {petID}
          </Typography>
          <Typography variant="h5" component="div">
            {petType}
          </Typography>
          <Typography sx={{ mb: 1.5 }} color="text.secondary">
            Last Fed: {lastFed ? moment(lastFed).fromNow() : "Never Fed"}
          </Typography>
          <Typography variant="body2">
            Currently Feeding: {latestRequest || feedClicked ? "Yes" : "No"}
          </Typography>
        </CardContent>
        <CardActions>
          <Button
            size="small"
            onClick={onFeedHandler}
            disabled={feedClicked || latestRequest ? true : false}
          >
            {feedClicked || latestRequest ? "Feeding..." : "Feed"}
          </Button>
        </CardActions>
      </Card>
      <Snackbar
        open={message !== ""}
        autoHideDuration={6000}
        onClose={() => setMessage("")}
      >
        <Alert
          onClose={() => setMessage("")}
          severity="success"
          sx={{ width: "100%" }}
        >
          {message}
        </Alert>
      </Snackbar>
    </>
  );
};

export default Pet;
