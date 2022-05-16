import React, { useEffect, useState } from "react";

// import { BrowserRouter as Router, Routes, Route } from "react-router-dom";

import axios from "axios";
import Pet from "./Pet";

const Pets = () => {
  const [pets, setPets] = useState([]);

  useEffect(() => {
    const getAllPets = async () => {
      let allPets = [];
      try {
        allPets = await axios.get("https://pet-feeder-iot.herokuapp.com/pet");
        setPets(allPets.data.pets);
      } catch (err) {
        console.log(err);
        setPets([]);
      }
    };
    getAllPets();
  }, []);

  return (
    <div className="d-flex flex-column flex-md-row">
      {pets.map((pet, index) => (
        <div
          className={`${index === 0 ? "me3-2 mt-2" : "mx-2"} my-3`}
          key={pet._id}
        >
          <Pet
            petID={pet._id}
            petType={pet.petType}
            latestRequest={pet.latestRequest}
            lastFed={pet.lastFed}
          />
        </div>
      ))}
    </div>
  );
};

export default Pets;
