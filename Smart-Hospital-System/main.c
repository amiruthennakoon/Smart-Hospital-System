#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS_PER_WARD 20

const int SPECIALTY_IDS[NUM_SPECIALTIES] = {1, 2, 3, 4};
const char SPECIALTY_NAMES[NUM_SPECIALTIES][30] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const double BASE_CONSULTATION_FEES[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5000.00};
const int CONSULTATION_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 30};
const int DAILY_PATIENT_CAPS[NUM_SPECIALTIES] = {30, 20, 12, 10};

const int WARD_IDS[NUM_WARDS] = {1, 2, 3, 4};
const char WARD_NAMES[NUM_WARDS][30] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"};
const double WARD_DAILY_RATES[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_BED_CAPACITIES[NUM_WARDS] = {20, 10, 10, 5};

int bedOccupancy[NUM_WARDS][MAX_BEDS_PER_WARD] = {0};
int specialtyQueueCounts[NUM_SPECIALTIES] = {0};
int patientCount = 0;
char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int patientUrgencyLevels[MAX_PATIENTS];
int patientSpecialtyIDs[MAX_PATIENTS];
int patientIsAdmitted[MAX_PATIENTS];
int patientWardIDs[MAX_PATIENTS];
int patientDaysAdmitted[MAX_PATIENTS];
double patientFinalAmounts[MAX_PATIENTS];

void displayMenu(void);
void displayBedOccupancy(void);
void registerPatient(void);
double calculateWaitingTime(int specIndex);
double calculateSurcharge(double baseFee, int urgencyLevel);
double calculateWardCost(int isAdmitted, int wardID, int days);
double calculateGrossTotal(double baseFee, double surcharge, double wardCost);

void registerPatient(void) {
    if (patientCount >= MAX_PATIENTS) {
        printf("\n[Error] Maximum patient capacity reached (%d patients max).\n", MAX_PATIENTS);
        return;
    }

    int i = patientCount;

    printf("\n==================================================\n");
    printf("            PATIENT INTAKE REGISTRATION           \n");
    printf("==================================================\n");

    printf("Enter Patient Name: ");
    getchar();
    fgets(patientNames[i], sizeof(patientNames[i]), stdin);
    patientNames[i][strcspn(patientNames[i], "\n")] = '\0';

    printf("Enter Patient Age: ");
    scanf("%d", &patientAges[i]);

    do {
        printf("Enter Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &patientUrgencyLevels[i]);
        if (patientUrgencyLevels[i] < 1 || patientUrgencyLevels[i] > 3) {
            printf("[Invalid Input] Please enter 1, 2, or 3.\n");
        }
    } while (patientUrgencyLevels[i] < 1 || patientUrgencyLevels[i] > 3);

    printf("\nAvailable Doctor Specialties:\n");
    for (int s = 0; s < NUM_SPECIALTIES; s++){
        printf(" %d. %s (Fee: LKR %.2f | Cap: %d/day)\n",SPECIALTY_IDS[s],SPECIALTY_NAMES[s],BASE_CONSULTATION_FEES[s],DAILY_PATIENT_CAPS[s]);
    }
    do {
        printf("Select Specialty ID (1 to 4): ");
        scanf("%d", &patientSpecialtyIDs[i]);
        if (patientSpecialtyIDs[i] < 1 || patientSpecialtyIDs[i] > NUM_SPECIALTIES) {
            printf("[Invalid Input] Specialty ID must be between 1 and 4. \n");
        }
    } while (patientSpecialtyIDs[i] < 1 || patientSpecialtyIDs[i] > NUM_SPECIALTIES);

    int specIndex = patientSpecialtyIDs[i] - 1;
    specialtyQueueCounts[specIndex]++;

    printf("\n[Success] Assigned to %s! Current Queue Count: %d\n",SPECIALTY_NAMES[specIndex], specialtyQueueCounts[specIndex]);

    do {
        printf("\nIs Patient Admitted to Ward? (1 = Yes, 0 = No): ");
        scanf("%d", &patientIsAdmitted[i]);
        if (patientIsAdmitted[i] != 0 && patientIsAdmitted[i] != 1) {
            printf("[Invalid Input] Please enter 1 for Yes or 0 for No.\n");
        }
    } while (patientIsAdmitted[i] != 0 && patientIsAdmitted[i] != 1);

    if (patientIsAdmitted[i] == 1) {
        printf("\nAvailable Hospital Wards:\n");
        for (int w = 0; w < NUM_WARDS; w++) {
            printf("  %d. %s (Rate: LKR %.2f/day | Cap: %d beds)\n",
                   WARD_IDS[w],
                   WARD_NAMES[w],
                   WARD_DAILY_RATES[w],
                   WARD_BED_CAPACITIES[w]);
        }

        do {
            printf("Select Ward ID (1 to 4): ");
            scanf("%d", &patientWardIDs[i]);
            if (patientWardIDs[i] < 1 || patientWardIDs[i] > NUM_WARDS) {
                printf("[Invalid Input] Ward ID must be between 1 and 4.\n");
            }
        } while (patientWardIDs[i] < 1 || patientWardIDs[i] > NUM_WARDS);

        printf("Enter Number of Days Admitted: ");
        scanf("%d", &patientDaysAdmitted[i]);

        int wardIdx = patientWardIDs[i] - 1;
        int bedAssigned = -1;

        for (int b = 0; b < WARD_BED_CAPACITIES[wardIdx]; b++) {
            if (bedOccupancy[wardIdx][b] == 0) {
                bedOccupancy[wardIdx][b] = 1;
                bedAssigned = b + 1;
                break;
            }
        }

        if (bedAssigned != -1) {
            printf("[Success] Allocated to %s (Bed #%02d)\n", WARD_NAMES[wardIdx], bedAssigned);
        } else {
            printf("[Warning] Ward full! No beds available in %s. Reverting to Outpatient status.\n", WARD_NAMES[wardIdx]);
            patientIsAdmitted[i] = 0;
            patientWardIDs[i] = 0;
            patientDaysAdmitted[i] = 0;
        }
    } else {

        patientWardIDs[i] = 0;
        patientDaysAdmitted[i] = 0;
    }

    patientCount++;
    printf("\n[Success] Patient Intake Complete! Total Registered Patients: %d\n", patientCount);
}

double calculateWaitingTime(int specIndex){
    if (specIndex < 0 || specIndex >= NUM_SPECIALTIES) {
        return 0.0;
    }
    return (double)(specialtyQueueCounts[specIndex] * CONSULTATION_TIMES[specIndex]);
}
double calculateSurcharge(double baseFee, int urgencyLevel) {
    if (urgencyLevel == 2){
        return baseFee * 0.20;
    } else if (urgencyLevel == 3){
        return baseFee * 0.50;
    }
    return 0.0;
}
double calculateWardCost(int isAdmitted, int wardID, int days){
    if (isAdmitted == 1 && wardID >= 1 && wardID <= NUM_WARDS){
        return WARD_DAILY_RATES[wardID - 1] * (double)days;
    }
    return 0.0;
}

double calculateGrossTotal(double baseFee, double surcharge, double wardCost){
    return baseFee + surcharge + wardCost;
}

int main(void) {
    int choice = 0;

    while (choice != 5) {
        displayMenu();
        printf("Enter your choice (1-5): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting...\n");
            break;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayBedOccupancy();
                break;
            case 3:
                printf("\n[Pending] Priority Triage Sorting (Branch 4)\n");
                break;
            case 4:
                printf("\n[Pending] Performance Reports & Analytics (Branch 5)\n");
                break;
            case 5:
                printf("\nExiting Smart Hospital System. Goodbye!\n");
                break;
            default:
                printf("\n[Error] Invalid option! Please select between 1 and 5.\n");
        }
    }

    return 0;
}

void displayBedOccupancy(void) {
    printf("\n==================================================\n");
    printf("         HOSPITAL BED OCCUPANCY STATUS            \n");
    printf("==================================================\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        printf("\n%s (Total Capacity: %d beds):\n", WARD_NAMES[w], WARD_BED_CAPACITIES[w]);
        for (int b = 0; b < WARD_BED_CAPACITIES[w]; b++) {
            printf("[Bed %02d: %s] ", b + 1, bedOccupancy[w][b] == 1 ? "OCCUPIED" : "AVAILABLE");
            if ((b + 1) % 5 == 0) printf("\n");
        }
        printf("\n");
    }
}

void displayMenu(void) {
    printf("\n==================================================\n");
    printf("   SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM   \n");
    printf("==================================================\n");
    printf("1. Register Patient & Generate Bill\n");
    printf("2. View Bed Occupancy Matrix\n");
    printf("3. Display Priority Queue (Triage Sorting)\n");
    printf("4. Performance Reports & Analytics\n");
    printf("5. Exit\n");
    printf("==================================================\n");
}
